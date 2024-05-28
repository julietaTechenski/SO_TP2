docker start TPEFINAL
docker exec -ti TPEFINAL make clean -C/root/Toolchain
docker exec -ti TPEFINAL make clean -C/root/
docker exec -ti TPEFINAL make -C/root/Toolchain
docker exec -ti TPEFINAL make -C/root/
docker stop TPEFINAL
