BEGIN{
    FS = " "

    # Get Opts
    for (i = 1; i <= ARGC; i++) {
        if (ARGV[i] == "--n_pixel") {
            n_pixel = ARGV[i+1]
            delete ARGV[i]
            delete ARGV[i+1]
			i++
        } else if (ARGV[i] == "--n_reps") {
            n_reps = ARGV[i+1]
            delete ARGV[i]
            delete ARGV[i+1]
			i++
        } else if (ARGV[i] ~ /^--./) {
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
NR == 1{
	print $0
}
NR > 1 {
	delete r
    for ( i = 0; i < n_reps; i++ ) {
        # Get indexes to flip
        for ( j = 1; j <= n_pixel; j++) {
            if (j == 1) {
                r[i][int((NF/2)*rand())]= 1
				continue
            }
			do{
				n = int((NF/2)*rand())
			} while(n in r[i])
			r[i][n]=1
        }
    }
	print $0
	for (i in r){

		cad=""
		for(j = 1; j <= NF ; j++){
			if(j in r[i])
				cad= cad ($j*-1)"   "
			else
				cad = cad $j	"   "	
		}
		print cad
	}

}
