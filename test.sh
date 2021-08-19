
#########################################################
#                                                       #
#                        MD-5                           #
#                                                       #
#########################################################

test_md5 () {
    
    if ( diff <(./ft_ssl md5 $1) <(md5 $1) )
    then
        echo -e "\033[0;32m[OK]\033[0m md5 $1"
    else
        echo -e "\033[0;31m[ERROR]\033[0m md5 $1"
    fi
}

test_md5_stdin () {
    
    if ( diff <(echo $1 | ./ft_ssl md5 $2) <(echo $1 | md5 $2) )
    then
        echo -e "\033[0;32m[OK]\033[0m echo $1 | md5 $2"
    else
        echo -e "\033[0;31m[ERROR]\033[0m echo $1 | md5 $2"
    fi
}

test_md5 '-s hello'
test_md5 '-s 1234567890123456789012345678901234567890123456789012345'
test_md5 '-s 12345678901234567890123456789012345678901234567890123456'
test_md5 '-s 1234567890123456789012345678901234567890123456789012345678901234'
test_md5 '-s 12345678901234567890123456789012345678901234567890123456789012345'
test_md5 '-s ""'
test_md5 '-q -s hello'
test_md5 '-r -s hello'
test_md5 '-qr -s hello'

test_md5_stdin 'hello'
test_md5_stdin 'hello' '-p'
test_md5_stdin 'hello' '-q'
test_md5_stdin 'hello' '-r'
test_md5_stdin 'hello' '-pq'
test_md5_stdin 'hello' '-pr'
test_md5_stdin 'hello' '-qr'
test_md5_stdin 'hello' '-pqr'
test_md5_stdin 'hello' '-p -s hello'
test_md5_stdin 'hello' '-pq -s hello'
test_md5_stdin 'hello' '-pr -s hello'
test_md5_stdin 'hello' '-pqr -s hello'

test_md5 '**/*.c Makefile'

#########################################################
#                                                       #
#                       SHA-256                         #
#                                                       #
#########################################################

test_sha256 () {
    
    if ( diff <(./ft_ssl sha256 $1) <(openssl sha256 $1) )
    then
        echo -e "\033[0;32m[OK]\033[0m sha256 $1"
    else
        echo -e "\033[0;31m[ERROR]\033[0m sha256 $1"
    fi
}

test_sha256_stdin () {
    
    if ( diff <(echo $1 | ./ft_ssl sha256 $2) <(echo $1 | openssl sha256 $2) )
    then
        echo -e "\033[0;32m[OK]\033[0m echo $1 | sha256 $2"
    else
        echo -e "\033[0;31m[ERROR]\033[0m echo $1 | sha256 $2"
    fi
}

# test_sha256 '-s hello'
# test_sha256 '-s 1234567890123456789012345678901234567890123456789012345'
# test_sha256 '-s 12345678901234567890123456789012345678901234567890123456'
# test_sha256 '-s 1234567890123456789012345678901234567890123456789012345678901234'
# test_sha256 '-s 12345678901234567890123456789012345678901234567890123456789012345'
# test_sha256 '-s ""'
# test_sha256 '-q -s hello'
# test_sha256 '-r -s hello'
# test_sha256 '-qr -s hello'

test_sha256_stdin 'hello'
test_sha256_stdin '1234567890123456789012345678901234567890123456789012345'
test_sha256_stdin '12345678901234567890123456789012345678901234567890123456'
test_sha256_stdin '1234567890123456789012345678901234567890123456789012345678901234'
test_sha256_stdin '12345678901234567890123456789012345678901234567890123456789012345'

# test_sha256_stdin 'hello' '-p'
# test_sha256_stdin 'hello' '-q'
# test_sha256_stdin 'hello' '-r'
# test_sha256_stdin 'hello' '-pq'
# test_sha256_stdin 'hello' '-pr'
# test_sha256_stdin 'hello' '-qr'
# test_sha256_stdin 'hello' '-pqr'
# test_sha256_stdin 'hello' '-p -s hello'
# test_sha256_stdin 'hello' '-pq -s hello'
# test_sha256_stdin 'hello' '-pr -s hello'
# test_sha256_stdin 'hello' '-pqr -s hello'

test_sha256 '**/*.c Makefile'

