#!/bin/sh
NOTIFYPATH=/home/tkonno/disk
TIMEOUT=5
MAILTO=root
/usr/bin/inotifywait -e create,delete,close \
    -mrq ${NOTIFYPATH} | \
while read line; do
    pars=(`echo ${line}`)
    status=${pars[1]}
    exe=`echo ${pars[2]} | sed 's/^.*\.\([^\.]*\)$/\1/'`
    if [[ "$status" =~ ^CLOSE_WRITE ]]; then
	if [ $exe = "dat" ]; then
	    echo "close "${pars[2]}
	    datname=${pars[2]}
	    filename=${datname%.dat}
	    echo "#!/bin/bash" > ${filename}.sh
	    echo "" >> ${filename}.sh
	    echo "cd /home/tkonno/jsns2_online/" >> ${filename}.sh
	    echo "source setenv" >> ${filename}.sh
	    echo "./bin/converter ${NOTIFYPATH}/${datname} ${NOTIFYPATH}/${filename}.root" >> ${filename}.sh
	    qsub -q jsns2 ${filename}.sh 
	    echo "submit conversion job ${filename}.sh"
	fi
    fi
    if [[ "$status" =~ ^CREATE ]]; then
	if [ $exe = "dat" ]; then
	    echo "create "${pars[2]}
	fi
    fi
done
