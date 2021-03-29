/*
 * This is a demonstration program for hss
 *
 * It is a simple file sign/verify utility: it is used as follows:
 *
 *   demo genkey keyname
 *       This creates a public/private key; it places the private key into
 *       keyname.prv, it places the public key into keyname.pub, and it places
 *       the auxiliary data into keyname.aux
 *   demo genkey keyname  15/4,10/8:2000
 *       This does the same, but specifies the parmaeter set to use.  In this
 *       example, it states that we have two Merkle levels, the top has 15
 *       levels (and uses Winternitz parameter 4), the bottom has 10 levels
 *       (and uses Winternitz parmaeter 8); up to 2000 bytes of aux data are
 *       used.  If you don't include the /x (Winternitz parameter) or the
 *       :2000 (aux data size), reasonable defaults are used
 *   demo genkey keyname 15/4,10/8:2000 seed=0123456789abcdef i=fedcba98765432
 *       This does the same, but instead of selecting a random seed and i
 *       value, this uses the specified values for the top-level LMS tree
 *       This is here to generate test vectors, not for real use
 *   demo sign keyname file.1 file.2 ... file.n
 *       This loads the private key keyname.prv (using keyname.aux if present)
 *       and then signs the files, producing the detached signatures
 *       file.1.sig, file.2.sig, ..., file.n.sig
 *       It also updates the keyname.prv file to reflect the generated
 *       signatures
 *   demo sign-bulk keyname Folder
 *       This loads the private key keyname.prv (using keyname.aux if present)
 *       and then signs all the files in the given folder and its subfolders,
 *       producing the detached signatures
 *       It also updates the keyname.prv file to reflect the generated
 *       signatures
 *   demo verify keyname file.1 file.2 ... file.n
 *       This takes the public key in keyname.pub, and uses it to verify
 *       whether file.1.sig is a valid signature for file.1, file.2.sig is
 *       a valid signature for file.2, etc
 *   demo verify-bulk keyname Folder
 *       This takes the public key in keyname.pub and uses it to verify
 *       whether all the files in the Folder and its subfolders have a
 *       valid signatures for the files
 *   demo advance keyname [integer]
 *       This takes the private key keyname.prf, and advances it [integer]
 *       places; that is, makes it assume it has generated [integer]
 *       signatures (without doing the work)
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include "hss.h"
#include "hss_verify_inc.h"
#include "hss_sign_inc.h"

#include "hash.h"
#include "hss_zeroize.h"

   /* When we generate a key, these are the parameters we use (unless the */
   /* user has specified otherwise). For signature generation/verification, */
   /* we use the parameters from the private key (for signature */
   /* generation) or the public key and signature (for verification) */

   /* By default, we use Merkle trees with two levels */
   /* Top tree has 20 levels, bottom tree has 10 (and so it's 20/10 in the */
   /* notation we use elsewhere) */
   /* We use Winternitz 8 for both trees; this minimizes the signature size */
   /* This gives us a reasonable genkey time (3 minutes with threading), */
   /* good load times (perhaps 1 second), and a billion signatures per key */
const char *default_parm_set = "20/8,10/8";

#define DEFAULT_AUX_DATA 10916   /* Use 10+k of aux data (which works well */
                            /* with the above default parameter set) */

static const char *seedbits = 0;
static const char *i_value = 0;
// static bool convert_specified_seed_i_value( void *, size_t );
static void usage(char *program);

static void usage(char *program) {
    printf( "Usage:\n" );
    printf( " %s genkey <keyname> [parameter set]\n", program );
    printf( " %s sign <keyname> [files to sign]\n", program );
    printf( " %s sign-bulk <keyname> <folder to sign>\n", program );
    printf( " %s verify <keyname> [files to verify]\n", program );
    printf( " %s verify-bulk <keyname> <folder to verify>\n", program );
    printf( " %s advance <keyname> <amount of advance>\n", program );
}

static const char *check_prefix( const char *s, const char *prefix ) {
    while (*prefix) {
        if (*s++ != *prefix++)
            return 0;
    }
    return s;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        usage(argv[0]);
        return 0;
    }
    if (0 == strcmp( argv[1], "genkey" )) {
        const char *keyname = 0;
        const char *parmname = 0;
        int i;
        for (i=2; i<argc; i++) {
            const char *s;
            if ((s = check_prefix( argv[i], "seed=" ))) {
                if (seedbits) {
                    printf( "Error: seed specified twice\n" );
                    return 0;
                }
                seedbits = s;
                continue;
            }
            if ((s = check_prefix( argv[i], "i=" ))) {
                if (i_value) {
                    printf( "Error: i specified twice\n" );
                    return 0;
                }
                i_value = s;
                continue;
            }
            if (!keyname) {
                keyname = argv[i];
                continue;
            }
            if (!parmname) {
                parmname = argv[i];
                continue;
            }
            printf( "Error: unexpected argument after parmset\n" );
            usage(argv[0]);
            return 0;
        }
        if (!keyname) {
            printf( "Error: missing keyname argument\n" );
            usage(argv[0]);
            return 0;
        }
        if (!seedbits != !i_value) {
            printf( "Error: must either specified both seed and i, or neither\n" );
            return 0;
        }

        // if (!keygen( keyname, parmname )) {
        //     printf( "Error creating keys\n" );
        // }
        return 0;
    }
    if (0 == strcmp( argv[1], "sign" )) {
        if (argc < 4) {
            printf( "Error: mssing keyname and file argument\n" );
            usage(argv[0]);
            return 0;
        }
        // if (!sign( argv[2], &argv[3] )) {
        //     printf( "Error signing\n" );
        // }
        return 0;
    }
    if (0 == strcmp( argv[1], "sign-bulk")) {
        if (argc != 4) {
            if (argc < 4) {
                printf("Error: missing keyname and folder argument\n");
            } else {
                printf("Error: Too many arguments.\n");
            }
            usage(argv[0]);
            return 0;
        }
        // Do bulk sign operation

        return 0;
    }
    if (0 == strcmp( argv[1], "verify" )) {
        if (argc < 4) {
            printf( "Error: mssing keyname and file argument\n" );
            usage(argv[0]);
            return 0;
        }
        // if (!verify( argv[2], &argv[3] )) {
        //     printf( "Error verifying\n" );
        // }
        return 0;
    }
    if (0 == strcmp( argv[1], "verify-bulk" )) {
        if (argc != 4) {
            if (argc < 4) {
                printf("Error: missing keyname and folder argument\n");
            } else {
                printf("Error: Too many arguments.\n");
            }
            usage(argv[0]);
            return 0;
        }
        // Do bulk verification operation

        return 0;
    }
    if (0 == strcmp( argv[1], "advance" )) {
        if (argc != 4) {
            printf( "Error: mssing amount to device the file\n" );
            usage(argv[0]);
            return 0;
        }
        // if (!advance( argv[2], argv[3] )) {
        //     printf( "Error advancing\n" );
        // }
        return 0;
    }

    usage(argv[0]);
    return 0;
}