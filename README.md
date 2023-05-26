# Games AI

## What is it?
This is a fast-api backend web service that enables the stratrgy game AI bots to interact with the UI at http://play.gamesai.co.in/

While the web app is built in python, the AI Bot itself is coded in c++ due to its better performance. Bridging of python and cpp was achieved by *ctypes*. More about it [here](https://docs.python.org/3/library/ctypes.html).

The AI bot utilizes the *minimax search*, an algorithm that formed the basis of [Deep Blue's](https://www.ibm.com/ibm/history/ibm100/us/en/icons/deepblue) victory over Kasparov, a renowned chess grandmaster.

The UI itself is a seperate reactjs web application - https://github.com/cant12/games-ai-UI

## How do I set up?
Create and activate a virtual environment with python 3.11, preferably using conda. One can install conda by following the steps [here](https://developers.google.com/earth-engine/guides/python_install-conda)
```
conda create -n games-ai python=3.11
conda activate games-ai
```

Install python requirements
```
pip install -r requirements.txt
```

Launch the application using the launch script
```
./launchscript.sh
```

**Note**: Make sure that g++ compiler is installed. This is required to compile the c++ component of the code.

## How do I deploy?
The web server is currently hosted in the aws ec2 instance. At this stage, the deployment is mostly mannual. We might automate it based on future needs.

Get the permission file ("games-ai-key-pair.pem") from the owner and save it in your .ssh folder. 

Then, login to the ec2 machine using the following command
```
./gamesai-ssh
```

Once you are in, pull the latest code and restart the server using the following steps

```
cd /opt/games-ai

git checkout main
git pull origin main

pm2 restart GamesAIBackend
```