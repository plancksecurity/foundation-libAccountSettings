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
		return as;
	
	// create resolver
	status = ldns_resolver_new_frm_file( &res, nullptr );  // nullptr == use system default, e.g. /etc/resolv.conf 
	if( status != LDNS_STATUS_OK)
		goto end;
	
	// DNS packet
	p = ldns_resolver_search(res,
	                         dom,
	                         LDNS_RR_TYPE_SRV,
	                         LDNS_RR_CLASS_IN,
	                         LDNS_RD); // RD = recursion desired
	
	// list of DNS Resource Records (RR)
	rr = ldns_pkt_rr_list_by_type(p,
	                              LDNS_RR_TYPE_SRV,
	                              LDNS_SECTION_ANSWER);
	

end:
	ldns_rr_list_sort(rr);
	ldns_rr_list_print(stdout, rr);
	ldns_rr_list_deep_free(rr);
	ldns_resolver_free( res );
	return as;
}

