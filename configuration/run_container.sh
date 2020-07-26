BASEDIR=$(dirname $(realpath $0))
interactive=""

if [[ "$#" -eq "0" ]] ; then	
	interactive="-it"
fi

docker run \
       ${interactive} \
       --rm \
       -v "$BASEDIR"/..:/repo \
       -w /repo\
       --name concurency_cont \
       concurency_image \
       "$@"
