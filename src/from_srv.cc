#include "from_srv.hh"
#include <ldns/ldns.h>
#include <stdexcept>
#include <memory>

namespace
{

	template<class T>
	using C_Ptr = std::unique_ptr<T, void(*)(T*)>;

	struct DNS_error : public std::runtime_error
	{
		DNS_error(const std::string& cause, const std::string& domain)
		: runtime_error(cause + " for domain \"" + domain + "\"")
		{}
	};

struct SRV
{
	unsigned priority;
	unsigned weight;
	unsigned port;
	std::string hostname;
	
	bool is_valid() const
	{
		return hostname.size() > 0;
	}
};


SRV get_srv_server(const std::string& domain, const std::string& service)
{
	ldns_resolver* res    = nullptr;
	ldns_pkt* p           = nullptr;
	ldns_rr_list*  rr     = nullptr;  // list of DNS Resource Records (RR)
	const ldns_rr* rr0 = nullptr; 
	ldns_status    status;
	
	const std::string service_domain = service + "." + domain;
	
	// RDATA
	C_Ptr<ldns_rdf> dom { ldns_dname_new_frm_str( service_domain.c_str() ), &ldns_rdf_free };
	if(!dom)
	{
		throw DNS_error("dom==nullptr!", domain);
	}

#if 0
	if (! ldns_dname_str_absolute(domain.c_str()) && ldns_dname_absolute(dom.get()))
	{
		fprintf(stderr, "not absolute.\n");
		/* ldns_dname_new_frm_str makes absolute dnames always!
		 * So deabsolutify domain.
		 * TODO: Create ldns_dname_new_frm_str_relative? Yuck!
		 */
		ldns_rdf_set_size(dom.get(), ldns_rdf_size(dom.get()) - 1);
	}
#endif
	
	// create resolver
	status = ldns_resolver_new_frm_file( &res, nullptr );  // nullptr == use system default, e.g. /etc/resolv.conf 
	if( status != LDNS_STATUS_OK)
	{
		fprintf(stderr, "status = %ld.\n", (long)status);
		goto end;
	}
	
	// DNS packet
	p = ldns_resolver_search(res,
	                         dom.get(),
	                         LDNS_RR_TYPE_SRV,
	                         LDNS_RR_CLASS_IN,
	                         LDNS_RD); // RD = recursion desired
	
	if(p == nullptr)
	{
		fprintf(stderr, "p==nullptr!\n");
		goto end;
	}
	
	
	// list of DNS Resource Records (RR)
	rr = ldns_pkt_rr_list_by_type(p,
	                              LDNS_RR_TYPE_SRV,
	                              LDNS_SECTION_ANSWER);
	
	if(rr == nullptr)
	{
		fprintf(stderr, "rr==nullptr!\n");
		goto end;
	}

	
	if(ldns_rr_list_rr_count(rr)==0)
	{
		fprintf(stderr, "empty rr_list\n");
		goto end;
	}
	
	ldns_rr_list_sort(rr); // FIXME: sort by priority?
	rr0 = ldns_rr_list_rr( rr, 0 );  // get the 1st (=highest priority?) entry
	
	if( ldns_rr_get_type(rr0) != LDNS_RR_TYPE_SRV)
	{
		fprintf(stderr, "got unexpected RR type %u\n", unsigned( ldns_rr_get_type(rr0) ));
		goto end;
	}
	
	printf("Number of rd entries: %zu.\n", ldns_rr_rd_count(rr0) );
	
	{
	const ldns_rdf* rdf_prio = ldns_rr_rdf(rr0, 0);
	const unsigned priority = ldns_rdf_get_type(rdf_prio)==LDNS_RDF_TYPE_INT16 ? ldns_rdf2native_int16(rdf_prio) : -1;

	const ldns_rdf* rdf_weight = ldns_rr_rdf(rr0, 1);
	const unsigned weight = ldns_rdf_get_type(rdf_weight)==LDNS_RDF_TYPE_INT16 ? ldns_rdf2native_int16(rdf_weight) : -1;

	const ldns_rdf* rdf_port = ldns_rr_rdf(rr0, 2);
	const unsigned port = ldns_rdf_get_type(rdf_port)==LDNS_RDF_TYPE_INT16 ? ldns_rdf2native_int16(rdf_port) : -1;

	const ldns_rdf* rdf_host = ldns_rr_rdf(rr0, 3);
	ldns_buffer* buf_host = ldns_buffer_new(128);
	ldns_rdf2buffer_str_dname( buf_host, rdf_host);
	
	const std::string host = ldns_rdf_get_type(rdf_host)==LDNS_RDF_TYPE_DNAME ? std::string( (const char*)buf_host->_data, buf_host->_limit) : "{°?°}";
	ldns_buffer_free(buf_host);
	buf_host=nullptr;
	
	printf("***\tPrio: %u, Weight: %u, Port: %u, Host: \"%s\".\n", priority, weight, port, host.c_str());
	return SRV{ priority, weight, port, host };
	}
	ldns_rr_list_print(stdout, rr);

end:
	ldns_rr_list_deep_free(rr);
	ldns_resolver_free( res );
	throw -1;
}

} // end of anonymous namespace


AccountSettings* get_settings_from_srv(AccountSettings* as, const std::string& accountName, const std::string& domain, const std::string& provider)
{
	fprintf(stderr, "== IMAP ==\n");
	const SRV imap_srv = get_srv_server( domain, "_imap._tcp" );
	
	fprintf(stderr, "== SMTP ==\n");
	const SRV smtp_srv = get_srv_server( domain, "_submission._tcp" );
	
	if(imap_srv.is_valid())
	{
		as->incoming.name = imap_srv.hostname;
		as->incoming.port = imap_srv.port;
	}

	if(smtp_srv.is_valid())
	{
		as->outgoing.name = smtp_srv.hostname;
		as->outgoing.port = smtp_srv.port;
	}
	
	return as;
}
