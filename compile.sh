if [ "$1" == "LIST" ]; then
  MEMORY_MANAGER="LIST"
else
  MEMORY_MANAGER="BUDDY"
fi

docker start TPEFINAL
docker exec -ti TPEFINAL make clean -C/root/Toolchain
docker exec -ti TPEFINAL make clean -C/root/
docker exec -ti TPEFINAL make -C/root/Toolchain
docker exec -ti TPEFINAL make -C/root MEMORY_MANAGER=$MEMORY_MANAGER
