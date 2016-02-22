/****************************************************************************
 * examples/hello/hello_main.c
 *
 *   Copyright (C) 2008, 2011-2012 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <nuttx/mbedtls/ssl.h>
#include <nuttx/mbedtls/x509_crt.h>
#include <nuttx/mbedtls/entropy.h>
#include <nuttx/mbedtls/ctr_drbg.h>
#include <nuttx/mbedtls/md5.h>
#include <nuttx/mbedtls/net.h>
#include <nuttx/mbedtls/debug.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <netdb.h>

#include <apps/netutils/dnsclient.h>
#include <apps/system/conman.h>

static const char *pers = "mini_client";



static void my_debug( void *ctx, int level,
                      const char *file, int line,
                      const char *str )
{
    ((void) level);

    printf( "TLS: %s:%04d: %s\n", file, line, str );
}


enum exit_codes
{
    exit_ok = 0,
    ctr_drbg_seed_failed,
    ssl_config_defaults_failed,
    ssl_setup_failed,
    hostname_failed,
    socket_failed,
    connect_failed,
    x509_crt_parse_failed,
    ssl_handshake_failed,
    ssl_write_failed,
};



#define GET_REQUEST "GET / HTTP/1.0\r\n\r\n"

// 4433
#define PORT_BE 0x1151  
#define PORT_LE 0x5111
// 10.0.0.88
#define ADDR_BE 0x0A000058
#define ADDR_LE 0x5800000A


int mbedtls_entropyer( void *data,
                    unsigned char *output, size_t len, size_t *olen )
{
    
    printf("asking for entropy?");
        
    unsigned long timer = 123;
    ((void) data);
    *olen = 0;

    if( len < sizeof(unsigned long) )
        return( 0 );

    memcpy( output, &timer, sizeof(unsigned long) );
    *olen = sizeof(unsigned long);

    return( 0 );
}

static int mainn(void) {
    int ret = exit_ok;

  int retries = 0;
    struct conman_status_s status;
    mbedtls_net_context server_fd;
    struct sockaddr_in addr;

    struct conman_client_s client;
    uint32_t connid = -1;
    
    
    printf("conman_client_init\n");
    
    ret = conman_client_init(&client);
    if (ret < 0)
    {
        printf("init-1 fail: %d\n",ret);
        return 0;
    }

    
    printf("conman_client_request_connection\n");

  ret = conman_client_request_connection(&client, CONMAN_DATA, &connid);
  if (ret < 0)
    {
      printf("conman_client_request_connection failed\n");
      goto exit;
    }

  while (true)
    {
      struct timespec sleeptime;
        printf("trying...\n");
      ret = conman_client_get_connection_status(&client, &status);
      if (ret < 0)
        {
          printf("conman_client_get_connection_status failed\n");
          goto exit;
        }

      if (status.status == CONMAN_STATUS_ESTABLISHED)
        {
                            printf("connok\n");

          break;
        }

        if (++retries == 10)
        {
            printf("error asd %d\n",(int)status.status);
            ret = ERROR;
            break;
            goto exit;
        }

      sleeptime.tv_sec  = 2;
      sleeptime.tv_nsec = 0;

      ret = nanosleep(&sleeptime, NULL);
      if (ret < 0)
        {
          
          if (ret == ERROR)
            {
                
                printf("error1\n");
              break;
            }
        }
    }
    
    
    
    
    // ---------------------------------
    
    
    
    
    printf("init0\n");

    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_ssl_context ssl;
    mbedtls_ssl_config conf;

    printf("init1\n");


    mbedtls_ctr_drbg_init( &ctr_drbg );
    printf("init2\n");

    mbedtls_net_init( &server_fd );
    printf("init3\n");
    mbedtls_ssl_init( &ssl );
    printf("init4\n");
    mbedtls_ssl_config_init( &conf );
    printf("init5\n");

    mbedtls_entropy_init( &entropy );
    
    printf("addsrc\n");
    
    mbedtls_entropy_add_source( &entropy, mbedtls_entropyer, NULL,
                            4,
                            MBEDTLS_ENTROPY_SOURCE_STRONG );


    
    printf("init5.5\n");
    if( mbedtls_ctr_drbg_seed( &ctr_drbg, mbedtls_entropy_func, &entropy,
                       (const unsigned char *) pers, strlen( pers ) ) != 0 )
    {
        
        printf("entropy fail\n");
        ret = ctr_drbg_seed_failed;
        goto exit;
    }
    printf("init6\n");

    if( mbedtls_ssl_config_defaults( &conf,
                MBEDTLS_SSL_IS_CLIENT,
                MBEDTLS_SSL_TRANSPORT_STREAM,
                MBEDTLS_SSL_PRESET_DEFAULT ) != 0 )
    {
        
        printf("init6 fail\n");
        ret = ssl_config_defaults_failed;
        goto exit;
    }
    
    
    
    printf("init7\n");
    mbedtls_ssl_conf_dbg( &conf, my_debug, 0 );
    
    printf("init8\n");
    
    mbedtls_ssl_conf_rng( &conf, mbedtls_ctr_drbg_random, &ctr_drbg );

    printf("init9\n");

    int sslok = mbedtls_ssl_setup( &ssl, &conf );
    
    if( sslok != 0 )
    {
        
        printf("sslfail %d\n",sslok);
        ret = ssl_setup_failed;
        goto exit;
    }
    memset( &addr, 0, sizeof( addr ) );
    addr.sin_family = AF_INET;
    printf("ssl\n");

    ret = 1;
    addr.sin_port = *((char *) &ret) == ret ? PORT_LE : PORT_BE;
    addr.sin_addr.s_addr = *((char *) &ret) == ret ? ADDR_LE : ADDR_BE;
    ret = 0;

    printf("socket\n");
    if( ( server_fd.fd = socket( AF_INET, SOCK_STREAM, 0 ) ) < 0 )
    {
        
        printf("no_socket %d\n",errno);
        ret = socket_failed;
        goto exit;
    }

    printf("socket!\n");
    
    if( connect( server_fd.fd,
                (const struct sockaddr *) &addr, sizeof( addr ) ) < 0 )
    {
        
        printf("noconnect\n");
        ret = connect_failed;
        goto exit;
    }

        printf("connected\n");

    
    mbedtls_ssl_set_bio( &ssl, &server_fd, mbedtls_net_send, mbedtls_net_recv, NULL );

        printf("bio\n");
    if( mbedtls_ssl_handshake( &ssl ) != 0 )
    {
        
        printf("shakefial\n");
        ret = ssl_handshake_failed;
        goto exit;
    }
        printf("handshake\n");

    if( mbedtls_ssl_write( &ssl, (const unsigned char *) GET_REQUEST,
                         sizeof( GET_REQUEST ) - 1 ) <= 0 )
    {
        
        printf("init6\n");
        ret = ssl_write_failed;
        goto exit;
    }

        printf("written\n");
    mbedtls_ssl_close_notify( &ssl );

exit:
    printf("exit\n");
    mbedtls_net_free( &server_fd );
    printf("E1\n");

    mbedtls_ssl_free( &ssl );
    printf("E2\n");
    mbedtls_ssl_config_free( &conf );
    printf("E3\n");
    mbedtls_ctr_drbg_free( &ctr_drbg );
    printf("E4\n");
    mbedtls_entropy_free( &entropy );
    printf("E5\n");

    return( ret );
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int hello_main(int argc, char *argv[])
#endif
{
    int rett = mainn();
    printf("RETURN %d\n",rett);
    return 0;
    
}
