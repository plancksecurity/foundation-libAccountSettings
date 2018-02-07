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


/* // only for debugging
template<class T, void(*D)(T*)>
void Petze( T* obj)
{
	fprintf(stderr, "Petze: %s %p \n", typeid(T).name(), (void*)obj );
	D(obj);
}
*/


SRV get_srv_server(const std::string& domain, const std::string& service)
{
	ldns_resolver* res    = nullptr;
//	ldns_pkt* p           = nullptr;
//	ldns_rr_list*  rr     = nullptr;  // list of DNS Resource Records (RR)
	const ldns_rr* rr0 = nullptr; 
	ldns_status    status;
	
	const std::string service_domain = service + "." + domain;
	
	// RDATA
	C_Ptr<ldns_rdf> dom { ldns_dname_new_frm_str( service_domain.c_str() ), &ldns_rdf_deep_free };
	if(!dom)
	{
		throw DNS_error("dom==nullptr!", domain);
	}

/*
	if (! ldns_dname_str_absolute(domain.c_str()) && ldns_dname_absolute(dom.get()))
	{
		fprintf(stderr, "not absolute.\n");
		// ldns_dname_new_frm_str makes absolute dnames always!
		// So deabsolutify domain.
		// TODO: Create ldns_dname_new_frm_str_relative? Yuck!
		ldns_rdf_set_size(dom.get(), ldns_rdf_size(dom.get()) - 1);
	}
*/
	
	// create resolver
	status = ldns_resolver_new_frm_file( &res, nullptr );  // nullptr == use system default, e.g. /etc/resolv.conf 
	C_Ptr<ldns_resolver> res_ptr { res, &ldns_resolver_deep_free };
	
	if( status != LDNS_STATUS_OK)
	{
		throw DNS_error("ldns_resolver_new_from_file returned " + std::to_string(status), domain );
	}
	
	// DNS packet
	C_Ptr<ldns_pkt> p { ldns_resolver_search(res,
	                         dom.get(),
	                         LDNS_RR_TYPE_SRV,
	                         LDNS_RR_CLASS_IN,
	                         LDNS_RD) // RD = recursion desired
	                    , & ldns_pkt_free };
	
	if(!p)
	{
		throw DNS_error("p==nullptr!", domain);
	}
	
	
	// list of DNS Resource Records (RR)
	C_Ptr<ldns_rr_list> rr {ldns_pkt_rr_list_by_type(p.get(),
	                              LDNS_RR_TYPE_SRV,
	                              LDNS_SECTION_ANSWER)
	                        , & ldns_rr_list_deep_free };
	
	if(!rr)
	{
		throw DNS_error("rr==nullptr!", domain);
	}

	
	if(ldns_rr_list_rr_count(rr.get())==0)
	{
		throw DNS_error("empty rr_list", domain);
	}
	
	ldns_rr_list_sort(rr.get()); // FIXME: sort by priority?
	rr0 = ldns_rr_list_rr( rr.get(), 0 );  // get the 1st (=highest priority?) entry
	
	if( ldns_rr_get_type(rr0) != LDNS_RR_TYPE_SRV)
	{
		throw DNS_error("got unexpected RR type " + std::to_string( unsigned( ldns_rr_get_type(rr0) )), domain);
	}
	
//	printf("Number of rd entries: %zu.\n", ldns_rr_rd_count(rr0) );
	
	const ldns_rdf* rdf_prio = ldns_rr_rdf(rr0, 0);
	const unsigned priority = ldns_rdf_get_type(rdf_prio)==LDNS_RDF_TYPE_INT16 ? ldns_rdf2native_int16(rdf_prio) : -1;

	const ldns_rdf* rdf_weight = ldns_rr_rdf(rr0, 1);
	const unsigned weight = ldns_rdf_get_type(rdf_weight)==LDNS_RDF_TYPE_INT16 ? ldns_rdf2native_int16(rdf_weight) : -1;

	const ldns_rdf* rdf_port = ldns_rr_rdf(rr0, 2);
	const unsigned port = ldns_rdf_get_type(rdf_port)==LDNS_RDF_TYPE_INT16 ? ldns_rdf2native_int16(rdf_port) : -1;

	const ldns_rdf* rdf_host = ldns_rr_rdf(rr0, 3);
	ldns_buffer* buf_host = ldns_buffer_new(128);
	ldns_rdf2buffer_str_dname( buf_host, rdf_host);
	
	std::string host = ldns_rdf_get_type(rdf_host)==LDNS_RDF_TYPE_DNAME ? std::string( (const char*)buf_host->_data, buf_host->_position) : "{°?°}";
	ldns_buffer_free(buf_host);
	buf_host=nullptr;
	
	while( (host.size() > 0) && (host.back() == '.') )
	{
		host.pop_back();
	}
	
//	printf("***\tPrio: %u, Weight: %u, Port: %u, Host: \"%s\".\n", priority, weight, port, host.c_str());
	
	return SRV{ priority, weight, port, host };
}

} // end of anonymous namespace


AccountSettings* get_settings_from_srv(AccountSettings* as, const std::string& accountName, const std::string& domain, const std::string& provider)
{
	if(as==nullptr)
	{
		throw std::runtime_error("get_settings_from_srv shall not be called with NULL pointer!");
	}
	
	bool imap_okay = false;
	bool smtp_okay = false;

//	fprintf(stderr, "== IMAP ==\n");
	try{
		const SRV imap_srv = get_srv_server( domain, "_imap._tcp" );
		if(imap_srv.is_valid())
		{
			as->incoming.name = imap_srv.hostname;
			as->incoming.port = imap_srv.port;
			imap_okay = true;
		}
	}catch( const DNS_error& )
	{
		// ignore unsuccessful DNS queries
	}

//	fprintf(stderr, "== SMTP ==\n");
	try{
		const SRV smtp_srv = get_srv_server( domain, "_submission._tcp" );
		if(smtp_srv.is_valid())
		{
			as->outgoing.name = smtp_srv.hostname;
			as->outgoing.port = smtp_srv.port;
			smtp_okay = true;
		}
	}catch( const DNS_error& )
	{
		// ignore unsuccessful DNS queries
	}
	
	if(imap_okay && smtp_okay)
	{
		as->status = AS_OK;
	}
	
	return as;
}
