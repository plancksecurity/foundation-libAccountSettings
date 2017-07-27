#include "from_srv.hh"
#include <ldns/ldns.h>

AccountSettings* get_settings_from_srv(AccountSettings* as, const std::string& accountName, const std::string& domain, const std::string& provider)
{
	ldns_resolver* res    = nullptr;
	ldns_pkt* p           = nullptr;
	ldns_rr_list*  rr     = nullptr;  // list of DNS Resource Records (RR)
	ldns_status    status;
	
	// RDATA
	ldns_rdf* dom = ldns_dname_new_frm_str( domain.c_str() );
	if(dom == nullptr)
	{
		fprintf(stderr, "dom==nullptr!\n");
		return as;
	}
	
	if (! ldns_dname_str_absolute(domain.c_str()) && ldns_dname_absolute(dom))
	{
		/* ldns_dname_new_frm_str makes absolute dnames always!
		 * So deabsolutify domain.
		 * TODO: Create ldns_dname_new_frm_str_relative? Yuck!
		 */
		ldns_rdf_set_size(dom, ldns_rdf_size(dom) - 1);
	}
	
	// create resolver
	status = ldns_resolver_new_frm_file( &res, nullptr );  // nullptr == use system default, e.g. /etc/resolv.conf 
	if( status != LDNS_STATUS_OK)
	{
		fprintf(stderr, "status = %ld.\n", (long)status);
		goto end;
	}
	
	// DNS packet
	p = ldns_resolver_search(res,
	                         dom,
	                         LDNS_RR_TYPE_MX,
	                         LDNS_RR_CLASS_IN,
	                         LDNS_RD); // RD = recursion desired
	
	if(p == nullptr)
	{
		fprintf(stderr, "p==nullptr!\n");
		goto end;
	}
	
	
	// list of DNS Resource Records (RR)
	rr = ldns_pkt_rr_list_by_type(p,
	                              LDNS_RR_TYPE_MX,
	                              LDNS_SECTION_ANSWER);
	
	if(rr == nullptr)
	{
		fprintf(stderr, "rr==nullptr!\n");
		goto end;
	}

	ldns_rr_list_sort(rr);
	ldns_rr_list_print(stdout, rr);

end:
	ldns_rr_list_deep_free(rr);
	ldns_resolver_free( res );
	return as;
}

