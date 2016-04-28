ENTRADA=$1
SALIDA=$2
NA=$3
NS=$4

gawk -F " " -v na=$NA -v ns=$NS '
	NR>0{
		if(NR <= ns+na){
			data[ns+na-NR]=$1
		}
		else{
			cad=data[ns+na-1]
			data[ns+na-1]=data[ns+na-2]
			for(i=ns+na-2 ; i>0; i--){
				cad=cad" "data[i]
				data[i]=data[i-1]			
			}
			cad=cad" "data[0]
			print cad
			data[0]=$1
		}
	}
	END{
		cad=data[ns+na-1]
		for(i=ns+na-2 ; i>=0; i--){
			cad=cad" "data[i]
		}
		print cad
	}

' $ENTRADA > $SALIDA
