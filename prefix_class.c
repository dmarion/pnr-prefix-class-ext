#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "dex.h"

#define OPTION_PREFIX_CLASS 200

extern int DEXAPI
prefix_class_initentry( int iExtensionPoint, dex_AttributeDictionary_t* pReq,
    dex_AttributeDictionary_t* pRes, dex_EnvironmentDictionary_t* pEnv)
{
	if (iExtensionPoint == DEX_INITIALIZE) {
		const char *psInitArgs = pEnv->get(pEnv, "arguments" );
		const char *psVersion = pEnv->get(pEnv,
		    "server-extensionapi-version");

		pEnv->log( pEnv, DEX_LOG_INFO, "prefix_class_initentry:"
		    "init (args: %s)", psInitArgs ? psInitArgs : "", NULL);

		if (psVersion && (atol(psVersion) > 1)) {
			pEnv->put(pEnv, "dhcp-support", "v6" );
			pEnv->put(pEnv, "extension-extensionapi-version",
			    psVersion);
		} else
			return DEX_ERROR;
	} else {
		pEnv->log(pEnv, DEX_LOG_INFO, "prefix_class_initentry:"
		    " uninitialized", NULL);
	}
	return DEX_OK;
}

extern int DEXAPI
prefix_class_set_class(int iExtensionPoint, dex_AttributeDictionary_t* pReq,
    dex_AttributeDictionary_t* pRes, dex_EnvironmentDictionary_t* pEnv)
{
	int count, i, j;

	if (!pRes->isValid(pRes) || !pRes->isV6(pRes))
		return DEX_ERROR;

	count = atol(pRes->getOption(pRes, DEX_HOME, DEX_OPTION_NAME,
	    "ia-pd", DEX_INSTANCE_COUNT, DEX_END ));

	for( i=0; i< count ; i++ ) {
		const abytes_t *pPD = pRes->getOptionBytes( pRes, DEX_HOME,
			    DEX_OPTION_NAME, "ia-pd", DEX_INSTANCE, i, DEX_MOVE_TO,
			    DEX_END );

		if(!pPD)
			break;

		for(j = 0; ; j++ ) {
			const abytes_t *pPrefix = pRes->getOptionBytes( pRes,
			    DEX_OPTION_NAME, "iaprefix",
			    DEX_INSTANCE, j,
			    DEX_END );

			if(!pPrefix)
				break;

			/* Repack IPv6 address and prefix */
			char buf[17];
			memcpy(&buf, &pPrefix->pBuffer[9], 16);
			buf[16] = pPrefix->pBuffer[8];

			if(pRes->setObject(pRes, DEX_LEASE, DEX_BY_IPV6PREFIX, 
			    &buf))
				break;

			const char *pszLeaseAddress = pRes->get( pRes,
			    "lease-ipaddress", 0, 0 );
			const char *pszPrefixName = pRes->get( pRes, 
			    "prefix-name", 0, 0 );
			const char *pszType = pRes->get( pRes, 
			    "lease-binding-type", 0, 0 );

			pEnv->log(pEnv, DEX_LOG_INFO, "Lease %s, Prefix %s,"
			    " Type %s", 
			    pszLeaseAddress ? pszLeaseAddress : "NULL", 
			    pszPrefixName ? pszPrefixName : "NULL",
			    pszType ? pszType : "NULL");

			uint16_t uPrefixClass = htons(j);
			abytes_t aPrefixClass;
			aPrefixClass.pBuffer = (unsigned char *) &uPrefixClass;
			aPrefixClass.cbBuffer = sizeof(uPrefixClass); 

			pRes->putOptionBytes(pRes, &aPrefixClass, 
			    DEX_OPTION_NAME, "iaprefix", 
			    DEX_INSTANCE, j,
			    DEX_OPTION_NUMBER, OPTION_PREFIX_CLASS,
			    DEX_END );
		}
		pRes->moveToOption( pRes, DEX_PARENT, DEX_END );
	}
	return DEX_OK;
}

