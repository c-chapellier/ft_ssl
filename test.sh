
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

if [[ $1 == '' || $1 == 'md5' ]]
then
    test_md5 '-s hello Makefile'
    test_md5 '-s 1234567890123456789012345678901234567890123456789012345'
    test_md5 '-s 12345678901234567890123456789012345678901234567890123456'
    test_md5 '-s 1234567890123456789012345678901234567890123456789012345678901234'
    test_md5 '-s 12345678901234567890123456789012345678901234567890123456789012345'
    test_md5 '-s ""'
    test_md5 '-q -s hello'
    test_md5 '-r -s hello'
    test_md5 '-qr -s hello'

    test_md5_stdin 'hello Makefile'
    test_md5_stdin 'hello' '-p'
    test_md5_stdin 'hello' '-p Makefile'
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
    test_md5_stdin 'hello' '-pqr -s hello Makefile'

    test_md5 '**/*.c Makefile'
fi

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

if [[ $1 == '' || $1 == 'sha256' ]]
then
    test_sha256_stdin 'hello Makefile'
    test_sha256_stdin '1234567890123456789012345678901234567890123456789012345'
    test_sha256_stdin '12345678901234567890123456789012345678901234567890123456'
    test_sha256_stdin '1234567890123456789012345678901234567890123456789012345678901234'
    test_sha256_stdin '12345678901234567890123456789012345678901234567890123456789012345'

    test_sha256 '**/*.c Makefile'
fi

#########################################################
#                                                       #
#                       BASE-64                         #
#                                                       #
#########################################################

test_base64 () {
    
    if ( diff <(./ft_ssl base64 $1) <(base64 $1) )
    then
        echo -e "\033[0;32m[OK]\033[0m base64 $1"
    else
        echo -e "\033[0;31m[ERROR]\033[0m base64 $1"
    fi
}

test_base64_stdin () {
    
    if ( diff <(echo $1 | ./ft_ssl base64 $2) <(echo $1 | base64 $2) )
    then
        echo -e "\033[0;32m[OK]\033[0m echo $1 | base64 $2"
    else
        echo -e "\033[0;31m[ERROR]\033[0m echo $1 | base64 $2"
    fi
}

test_base64_file_output () {
    
    if ( diff <(./ft_ssl base64 -o $1.mine $2) <(base64 -o $1.true $2) )
    then
        if ( diff $1.mine $1.true )
        then
            echo -e "\033[0;32m[OK]\033[0m base64 -o $1 $2"
        else
            echo -e "\033[0;31m[ERROR]\033[0m base64 -o $1 $2"
        fi
    else
        echo -e "\033[0;31m[ERROR]\033[0m base64 -o $1 $2"
    fi
}

test_base64_stdin_file_output () {
    
    if ( diff <(echo $1 | ./ft_ssl base64 -o $2.mine $3) <(echo $1 | base64 -o $2.true $3) )
    then
        if ( diff $2.mine $2.true )
        then
            echo -e "\033[0;32m[OK]\033[0m echo $1 | base64 -o $2 $3"
        else
            echo -e "\033[0;31m[ERROR]\033[0m echo $1 | base64 -o $2 $3"
        fi
    else
        echo -e "\033[0;31m[ERROR]\033[0m echo $1 | base64 -o $2 $3"
    fi
}

if [[ $1 == '' || $1 == 'base64' ]]
then
    test_base64_stdin ''
    test_base64_stdin '0'
    test_base64_stdin '01'
    test_base64_stdin '012'
    test_base64_stdin '0123'
    test_base64_stdin '01234'
    test_base64_stdin 'foobar'
    test_base64_stdin 'Zm9vYmFyCg==' '-d'
    test_base64_stdin 'Zm9vYmFy' '-d'
    test_base64_stdin '' '-d'

    test_base64 '-i Makefile'
    test_base64 '-i README.md'
    test_base64 '-i main.c'

    test_base64_file_output 'output' '-i Makefile'
    test_base64_file_output 'output2' '-i output.true -d'
    test_base64_stdin_file_output 'Zm9vYmFyCg==' 'output' '-d'
    test_base64_stdin_file_output 'Zm9vYmFy' 'output' '-d'
    test_base64_stdin_file_output 'foobar' 'output'

    rm -f output.mine output.true output2.mine output2.true
fi

#########################################################
#                                                       #
#                        DES                            #
#                                                       #
#########################################################

test_des () {
    
    if ( diff <($1) <($2) )
    then
        echo -e "\033[0;32m[OK]\033[0m $2"
    else
        echo -e "\033[0;31m[ERROR]\033[0m $2"
    fi
}

test_des_ecb_file_output () {

    ./ft_ssl des-ecb -k $2 -i $1 -o output
    
    if ( diff <(cat $1) <(./ft_ssl des-ecb -d -k $2 -i output) )
    then
        echo -e "\033[0;32m[OK]\033[0m test_des_ecb_file_output $1 $2"
    else
        echo -e "\033[0;31m[ERROR]\033[0m test_des_ecb_file_output $1 $2"
    fi
    rm -f output
}

test_des_ecb_file_output_a () {

    ./ft_ssl des-ecb -a -k $2 -i $1 -o output
    
    if ( diff <(cat $1) <(./ft_ssl des-ecb -d -a -k $2 -i output) )
    then
        echo -e "\033[0;32m[OK]\033[0m test_des_ecb_file_output_a $1 $2"
    else
        echo -e "\033[0;31m[ERROR]\033[0m test_des_ecb_file_output_a $1 $2"
    fi
    rm -f output
}

test_des_cbc_file_output () {

    ./ft_ssl des -v ffffffffffffffff -k $2 -i $1 -o output
    
    if ( diff <(cat $1) <(./ft_ssl des -d -v ffffffffffffffff -k $2 -i output) )
    then
        echo -e "\033[0;32m[OK]\033[0m test_des_ecb_file_output $1 $2"
    else
        echo -e "\033[0;31m[ERROR]\033[0m test_des_ecb_file_output $1 $2"
    fi
    rm -f output
}

test_des_cbc_file_output_a () {

    ./ft_ssl des -a -v ffffffffffffffff -k $2 -i $1 -o output
    
    if ( diff <(cat $1) <(./ft_ssl des -d -a -v ffffffffffffffff -k $2 -i output) )
    then
        echo -e "\033[0;32m[OK]\033[0m test_des_ecb_file_output_a $1 $2"
    else
        echo -e "\033[0;31m[ERROR]\033[0m test_des_ecb_file_output_a $1 $2"
    fi
    rm -f output
}

if [[ $1 == '' || $1 == 'des' ]]
then
    test_des './ft_ssl des-ecb -k ffffffffffffffff -i Makefile' 'openssl des-ecb -K ffffffffffffffff -in Makefile'
    test_des './ft_ssl des-ecb -k ffffffffffffffff -i README.md' 'openssl des-ecb -K ffffffffffffffff -in README.md'
    test_des './ft_ssl des-ecb -k ffffffffffffffff -i main.c' 'openssl des-ecb -K ffffffffffffffff -in main.c'
    test_des './ft_ssl des-ecb -k ffffffffffffffff -i ft_ssl.h' 'openssl des-ecb -K ffffffffffffffff -in ft_ssl.h'

    test_des './ft_ssl des-ecb -k 0123456789abcdef -i Makefile' 'openssl des-ecb -K 0123456789abcdef -in Makefile'
    test_des './ft_ssl des-ecb -k 0123456789abcdef -i README.md' 'openssl des-ecb -K 0123456789abcdef -in README.md'
    test_des './ft_ssl des-ecb -k 0123456789abcdef -i main.c' 'openssl des-ecb -K 0123456789abcdef -in main.c'
    test_des './ft_ssl des-ecb -k 0123456789abcdef -i ft_ssl.h' 'openssl des-ecb -K 0123456789abcdef -in ft_ssl.h'

    test_des './ft_ssl des-ecb -a -k ffffffffffffffff -i Makefile' 'openssl des-ecb -a -K ffffffffffffffff -in Makefile'
    test_des './ft_ssl des-ecb -a -k ffffffffffffffff -i README.md' 'openssl des-ecb -a -K ffffffffffffffff -in README.md'
    test_des './ft_ssl des-ecb -a -k ffffffffffffffff -i main.c' 'openssl des-ecb -a -K ffffffffffffffff -in main.c'
    test_des './ft_ssl des-ecb -a -k ffffffffffffffff -i ft_ssl.h' 'openssl des-ecb -a -K ffffffffffffffff -in ft_ssl.h'

    test_des './ft_ssl des-ecb -a -k 0123456789abcdef -i Makefile' 'openssl des-ecb -a -K 0123456789abcdef -in Makefile'
    test_des './ft_ssl des-ecb -a -k 0123456789abcdef -i README.md' 'openssl des-ecb -a -K 0123456789abcdef -in README.md'
    test_des './ft_ssl des-ecb -a -k 0123456789abcdef -i main.c' 'openssl des-ecb -a -K 0123456789abcdef -in main.c'
    test_des './ft_ssl des-ecb -a -k 0123456789abcdef -i ft_ssl.h' 'openssl des-ecb -a -K 0123456789abcdef -in ft_ssl.h'

    test_des_ecb_file_output Makefile ffffffffffffffff
    test_des_ecb_file_output README.md ffffffffffffffff
    test_des_ecb_file_output main.c ffffffffffffffff
    test_des_ecb_file_output ft_ssl.h ffffffffffffffff

    test_des_ecb_file_output Makefile 0123456789abcdef
    test_des_ecb_file_output README.md 0123456789abcdef
    test_des_ecb_file_output main.c 0123456789abcdef
    test_des_ecb_file_output ft_ssl.h 0123456789abcdef

    test_des_ecb_file_output_a Makefile ffffffffffffffff
    test_des_ecb_file_output_a README.md ffffffffffffffff
    test_des_ecb_file_output_a main.c ffffffffffffffff
    test_des_ecb_file_output_a ft_ssl.h ffffffffffffffff

    test_des_ecb_file_output_a Makefile 0123456789abcdef
    test_des_ecb_file_output_a README.md 0123456789abcdef
    test_des_ecb_file_output_a main.c 0123456789abcdef
    test_des_ecb_file_output_a ft_ssl.h 0123456789abcdef

    ###############################          CBC          ###############################

    test_des './ft_ssl des -v ffffffffffffffff -k ffffffffffffffff -i Makefile' 'openssl des -iv ffffffffffffffff -K ffffffffffffffff -in Makefile'
    test_des './ft_ssl des -v ffffffffffffffff -k ffffffffffffffff -i README.md' 'openssl des -iv ffffffffffffffff -K ffffffffffffffff -in README.md'
    test_des './ft_ssl des -v ffffffffffffffff -k ffffffffffffffff -i main.c' 'openssl des -iv ffffffffffffffff -K ffffffffffffffff -in main.c'
    test_des './ft_ssl des -v ffffffffffffffff -k ffffffffffffffff -i ft_ssl.h' 'openssl des -iv ffffffffffffffff -K ffffffffffffffff -in ft_ssl.h'
    
    test_des './ft_ssl des -v ffffffffffffffff -k 0123456789abcdef -i Makefile' 'openssl des -iv ffffffffffffffff -K 0123456789abcdef -in Makefile'
    test_des './ft_ssl des -v ffffffffffffffff -k 0123456789abcdef -i README.md' 'openssl des -iv ffffffffffffffff -K 0123456789abcdef -in README.md'
    test_des './ft_ssl des -v ffffffffffffffff -k 0123456789abcdef -i main.c' 'openssl des -iv ffffffffffffffff -K 0123456789abcdef -in main.c'
    test_des './ft_ssl des -v ffffffffffffffff -k 0123456789abcdef -i ft_ssl.h' 'openssl des -iv ffffffffffffffff -K 0123456789abcdef -in ft_ssl.h'
    
    test_des './ft_ssl des -v ffffffffffffffff -a -k ffffffffffffffff -i Makefile' 'openssl des -a -iv ffffffffffffffff -K ffffffffffffffff -in Makefile'
    test_des './ft_ssl des -v ffffffffffffffff -a -k ffffffffffffffff -i README.md' 'openssl des -a -iv ffffffffffffffff -K ffffffffffffffff -in README.md'
    test_des './ft_ssl des -v ffffffffffffffff -a -k ffffffffffffffff -i main.c' 'openssl des -a -iv ffffffffffffffff -K ffffffffffffffff -in main.c'
    test_des './ft_ssl des -v ffffffffffffffff -a -k ffffffffffffffff -i ft_ssl.h' 'openssl des -a -iv ffffffffffffffff -K ffffffffffffffff -in ft_ssl.h'
    
    test_des './ft_ssl des -v ffffffffffffffff -a -k 0123456789abcdef -i Makefile' 'openssl des -a -iv ffffffffffffffff -K 0123456789abcdef -in Makefile'
    test_des './ft_ssl des -v ffffffffffffffff -a -k 0123456789abcdef -i README.md' 'openssl des -a -iv ffffffffffffffff -K 0123456789abcdef -in README.md'
    test_des './ft_ssl des -v ffffffffffffffff -a -k 0123456789abcdef -i main.c' 'openssl des -a -iv ffffffffffffffff -K 0123456789abcdef -in main.c'
    test_des './ft_ssl des -v ffffffffffffffff -a -k 0123456789abcdef -i ft_ssl.h' 'openssl des -a -iv ffffffffffffffff -K 0123456789abcdef -in ft_ssl.h'

    test_des_cbc_file_output Makefile ffffffffffffffff
    test_des_cbc_file_output README.md ffffffffffffffff
    test_des_cbc_file_output main.c ffffffffffffffff
    test_des_cbc_file_output ft_ssl.h ffffffffffffffff

    test_des_cbc_file_output Makefile 0123456789abcdef
    test_des_cbc_file_output README.md 0123456789abcdef
    test_des_cbc_file_output main.c 0123456789abcdef
    test_des_cbc_file_output ft_ssl.h 0123456789abcdef

    test_des_cbc_file_output_a Makefile ffffffffffffffff
    test_des_cbc_file_output_a README.md ffffffffffffffff
    test_des_cbc_file_output_a main.c ffffffffffffffff
    test_des_cbc_file_output_a ft_ssl.h ffffffffffffffff
    
    test_des_cbc_file_output_a Makefile 0123456789abcdef
    test_des_cbc_file_output_a README.md 0123456789abcdef
    test_des_cbc_file_output_a main.c 0123456789abcdef
    test_des_cbc_file_output_a ft_ssl.h 0123456789abcdef
fi
