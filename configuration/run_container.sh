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
       --name cpp_training_module1 \
       training_module1_image \
       "$@"
