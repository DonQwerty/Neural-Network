BEGIN{
    FS = " "

    # Get Opts
    for (i = 1; i < ARGC; i++) {
        if (ARGV[i] == "-p") {
            n_pixel = ARGV[i+1]
            delete ARGV[i]
            delete ARGV[i+1]
        } else if (ARGV[i] == "-r") {
            n_reps = ARGV[i+1]
            delete ARGV[i]
            delete ARGV[i+1]
        } else if (ARGV[i] ~ /^-./) {
            e = sprintf("%s: unrecognized option -- %c",
                        ARGV[0], substr(ARGV[i], 2, 1))
            print e > "/dev/stderr"
        } else {
            break
        }
    }

    if ( n_pixel <= 0) {
        exit 1
    }
}
NR > 1 {

    for ( i = 0; i < n_rep; i++ ) {
        # Get indexes to flip
        for ( j = 0; j < n_pixel; j++) {
            if (j == 0) {
                r[i][j]
            }
        }
    }

    int(n * rand())
}
