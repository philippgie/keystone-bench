//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include <iostream>
#include <cstdio>
#include "keystone.h"
#include "edge_call.h"

int main(int argc, char** argv)
{
  if(argc != 7)
  {
    printf("Usage: %s <eapp> <runtime> <untrusted_size(K)> <freemem_size(K)> <self_timing> <load_only>\n", argv[0]);
    return 0;
  }

  size_t untrusted_size = atoi(argv[3])*1024;
  size_t freemem_size = atoi(argv[4])*1024;
  int self_timing = atoi(argv[5]);
  int load_only = atoi(argv[6]);

  Keystone enclave;
  Params params;

  params.setFreeMemSize(freemem_size);
  params.setUntrustedMem(DEFAULT_UNTRUSTED_PTR, untrusted_size);

  enclave.init(argv[1], argv[2], params);

  enclave.registerOcallDispatch(incoming_call_dispatch);
  edge_call_init_internals((uintptr_t)enclave.getSharedBuffer(),
			   enclave.getSharedBufferSize());


  printf("[keystone-bench] Params:\n\tuntrusted: %lu\n\tfreemem: %lu\n\t%s\n\t%s\n*********\n",untrusted_size,freemem_size,
         self_timing?"Performing internal timing":"No timing",
         load_only?"ONLY LOADING, not running":"Running benchmark");


  unsigned long cycles1,cycles2;
  if( self_timing ){
    asm volatile ("rdcycle %0" : "=r" (cycles1));
  }

  if( !load_only )
    enclave.run();

  if( self_timing ){
    asm volatile ("rdcycle %0" : "=r" (cycles2));
    printf("[keystone-bench] Runtime: %ul cycles\r\n", cycles2-cycles1);
  }

  return 0;
}
