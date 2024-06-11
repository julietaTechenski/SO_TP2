# TPE - 20232Q

### Environment setup

The following packages must be installed:

    sudo nasm qemu gcc make

### Docker setup

Execute the following commands to install Docker:
    
    sudo apt install docker.io

    sudo groupadd docker

    sudo usermod -aG docker $USER 

Once installed, download the image necessary:

    sudo docker pull agodio/itba-so:2.0

On the project directory, start the container:

    sudo docker run -v ${PWD}:/root --privileged -ti --name TPEFINAL agodio/itba-so:2.0



### Project compilation

In compilation time, user can set the MMU of choice :
    - Buddy
    - Free List
The default memory is Buddy.

Execute the following commands on the project directory (outside Docker):

    ./compile.sh

To run freeList MMU execute the following commands on the project directory (outside Docker):

    ./compile.sh LIST

### Project execution

From the project directory run:

    ./run.sh

### Inside project

Within the system, a window will pop up. The functionality of the command interpreter is based on different functions that can be invoked that will display certain information. To access their description, simply type “help” and press enter.

### Special keys
    F2 - displays the registers current values.
    F3 - undoes F2 function.
    Ctrl + C - kills process in foreground
    Ctrl + D - sends EOFILE

## Authors:
              Campoli, Lucas - 63295
              Taurian, Magdalena - 62828
              Techenski, Julieta - 62547


Collaborators: Rodrigo Rearden (RowDaBoat) and Augusto Nizzo McIntosh
