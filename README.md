# CSCI 3081W - Project - "The Arena"

## Overview

This is the base support code for the 2026 Spring CSCI 3081W Project - "The Arena".  Please refer to the [Iteration 1 Requirements Specification](https://github.umn.edu/umn-csci-3081-s26/public-docs/blob/main/Iteration1/README.md) for what we expect you and your team to develop.  Please also read the **Build Submission** instructions below to create a ```project.zip``` file to submit to the Gradescope autograder when it is available.

## Pre-requisites
  * [Git](https://git-scm.com/)

## Docker Pre-requisites
  * Windows 10 Home
    * Install [wsl2 and Ubuntu](https://www.youtube.com/watch?v=ilKQHAFeQR0&list=RDCMUCzLbHrU7U3cUDNQWWAqjceA&start_radio=1&t=7)
  * Install [Docker Desktop](https://hub.docker.com/?overlay=onboarding) from [Docker Hub](https://hub.docker.com/)
  * Linux
    * Use [docker group instead of sudo](https://www.digitalocean.com/community/tutorials/how-to-install-and-use-docker-on-ubuntu-18-04)

## Getting Started with Docker

1. Build docker image

    ```bash
    bin/build-env.sh
    ```

2. Run docker image

    ```bash
    #Usage bin/run-env.sh <port - optional(default 8081)>
    bin/run-env.sh
    ```
    
3. Build project web server (inside docker image) NOTE: You won't be able to `cd` here yet because the project directory does not exist. If you were able to launch the above commands you should now be inside the docker image. You can exit it with CTRL+D now.

    ```bash
    # Inside docker image
    make
    ```
    
4. Run web server (inside docker image)

    ```bash
    make run
    ```
    
5. Open up Firefox and browse to http://127.0.0.1:8081/


## Getting Started on Lab Machines

1. SSH to a Lab Machine (with port forwarding ```-L 8081:127.0.0.1:8081``` - See [SSH FAQ](https://github.umn.edu/umn-csci-3081-s26/FAQ/tree/main/SSH) for more details.

   ```bash
   ssh -L 8081:127.0.0.1:8081 x500@csel-xxxx.cselabs.umn.edu
   ```
   
   Example:
   ```bash
   ssh -L 8081:127.0.0.1:8081 x500@csel-kh1250-05.cselabs.umn.edu
   ```

2. Build project

    ```bash
    cd /path/to/cloned/project/repository
    make
    ```
    
3. Run project

    ```bash
    make run
    ```
    
4. Open up Firefox and browse to http://127.0.0.1:8081/


## Getting Started on Local Machine (Not Supported by CSCI 3081 Staff)

We can try to assist you here, but installing on local machines is technically not supported by CSCI 3081 Staff due to the complexities of individual environments.  We will however support both the Docker and Lab Machines, so these are recommended.

1. Install development dependencies.  Refer to the Dockefile for what would be need on a Linux system or WSL:

   * [Dependency List](Dockerfile#L7)

2. Build project

    ```bash
    cd /path/to/cloned/project/repository
    make
    ```
    
3. Run project

    ```bash
    make run
    ```
    
4. Open up Firefox and browse to http://127.0.0.1:8081/

## Build Submission

1. Build your submission for submitting to Gradescope.

   ```bash
   make submission
   ```

2. Submit ```project.zip``` to the Gradescope assignment.


## Running / Debugging

* Running with ```data/scenes/default.json```:
 
    ```bash
    # Running
    make run
    
    # Debugging with GDB
    make debug
    ```

* Setting a scene configuration (e.g. ```data/scenes/default.json```):

    ```bash
    # Running
    ./bin/start.sh data/scenes/default.json
    
    # Debugging with GDB
    ./bin/debug.sh data/scenes/robots.json
    ```

* Running with a different port (e.g. 8082 instead of 8081):
  
    Environment Setup:

    ```bash
    # Docker
    ./bin/run_env.sh 8082
    
    # SSH
    ssh -L 8082:127.0.0.1:8082 x500@csel-xxxx.cselabs.umn.edu
    ```
    Running:

    ```bash
    # Running
    ./bin/start.sh data/scenes/umn.json 8082
    
    # Debugging with GDB
    ./bin/debug.sh data/scenes/bv.json 8082
    ```


