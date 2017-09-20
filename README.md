# OpenBASN

**Open** **B**ody **A**rea **S**ensor **N**etwork

1. Instruções

    1. Clonar repositório

        ```
        git clone https://github.com/rdinizcal/OpenBASN.git
        ```

    2. Instalar OpenDaVinci

        If you are on Ubuntu 16.04 Linux distribution:

        1. Add the public key from our repository:

            ```
            $ wget -O - -q http://opendavinci.cse.chalmers.se/opendavinci.cse.chalmers.se.gpg.key | sudo apt-key add -
            ```

        2. Add our repository:

            ```
            $ sudo sh -c 'echo "deb [arch=amd64] http://opendavinci.cse.chalmers.se/ubuntu xenial main" > /etc/apt/sources.list.d/opendavinci.list'
            ```

        3. Update your package database:

            ```
            $ sudo apt-get update
            ```

        4. Install OpenDaVINCI:

            ```
            $ sudo apt-get install opendavinci-lib opendavinci-odtools opendavinci-odsupercomponent
            ```

        Otherwise:

        Consult http://opendavinci.readthedocs.io/en/latest/installation.pre-compiled.html#