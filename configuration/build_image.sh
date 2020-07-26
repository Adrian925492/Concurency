BASEDIR=$(dirname $(realpath $0))
docker build -t=concurency_image "$BASEDIR"

