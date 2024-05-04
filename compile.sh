sudo docker start TPEFINAL
sudo docker exec -ti TPEFINAL make clean -C/root/Toolchain
sudo docker exec -ti TPEFINAL make clean -C/root/
sudo docker exec -ti TPEFINAL make -C/root/Toolchain
sudo docker exec -ti TPEFINAL make -C/root/
sudo docker stop TPEFINAL
