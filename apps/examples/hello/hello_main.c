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


static const char *pers = "mini_client";

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

#define PORT_BE 0x1151      /* 4433 */
#define PORT_LE 0x5111
#define ADDR_BE 0x5B98ED02  /* 127.0.0.1 */
#define ADDR_LE 0x02ED985B


static int mainn(void) {
   int ret = exit_ok;
    mbedtls_net_context server_fd;
    struct sockaddr_in addr;

    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_ssl_context ssl;
    mbedtls_ssl_config conf;
    mbedtls_ctr_drbg_init( &ctr_drbg );

    mbedtls_net_init( &server_fd );
    mbedtls_ssl_init( &ssl );
    mbedtls_ssl_config_init( &conf );

    mbedtls_entropy_init( &entropy );
    if( mbedtls_ctr_drbg_seed( &ctr_drbg, mbedtls_entropy_func, &entropy,
                       (const unsigned char *) pers, strlen( pers ) ) != 0 )
    {
        ret = ctr_drbg_seed_failed;
        goto exit;
    }

    if( mbedtls_ssl_config_defaults( &conf,
                MBEDTLS_SSL_IS_CLIENT,
                MBEDTLS_SSL_TRANSPORT_STREAM,
                MBEDTLS_SSL_PRESET_DEFAULT ) != 0 )
    {
        ret = ssl_config_defaults_failed;
        goto exit;
    }
    printf("inited\n");
    mbedtls_ssl_conf_rng( &conf, mbedtls_ctr_drbg_random, &ctr_drbg );

    printf("rng\n");


    if( mbedtls_ssl_setup( &ssl, &conf ) != 0 )
    {
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
        ret = ssl_handshake_failed;
        goto exit;
    }
        printf("handshake\n");

    if( mbedtls_ssl_write( &ssl, (const unsigned char *) GET_REQUEST,
                         sizeof( GET_REQUEST ) - 1 ) <= 0 )
    {
        ret = ssl_write_failed;
        goto exit;
    }

        printf("written\n");
    mbedtls_ssl_close_notify( &ssl );

exit:
    mbedtls_net_free( &server_fd );

    mbedtls_ssl_free( &ssl );
    mbedtls_ssl_config_free( &conf );
    mbedtls_ctr_drbg_free( &ctr_drbg );
    mbedtls_entropy_free( &entropy );

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