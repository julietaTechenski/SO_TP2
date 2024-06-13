if [ "$1" == "BUDDY" ]; then
  MEMORY_MANAGER="BUDDY"
else
  MEMORY_MANAGER="LIST"
fi

docker start TPEFINAL
docker exec -ti TPEFINAL make clean -C/root/Toolchain
docker exec -ti TPEFINAL make clean -C/root/
docker exec -ti TPEFINAL make -C/root/Toolchain
docker exec -ti TPEFINAL make -C/root MEMORY_MANAGER=$MEMORY_MANAGER