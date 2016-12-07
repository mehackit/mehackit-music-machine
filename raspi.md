# Raspberry Pi

## Connecting with SSH via ethernet on OSX

- IP = 	192.168.2.2

- Install XQuartz

- Check internet preferences

- Plug the ethernet cable, plug power to the Raspberry Pi

- Terminal: ```ssh -X pi@192.168.2.2```

- Password: raspi

- Copy local pd-path to raspi:

  - Open a new terminal tab on your own computer and locate the file you want to send.
  - ```scp patch.pd pi@192.168.2.2:/some/remote/directory/ ```

- Open the patch on raspi:

  - ```pd-extended patch.pd```

- Open the patch on startup:

  - open .bashrc: ```nano ~/.bashrc```
  - add: ```pd-extended -nogui /path/to/patch.pd```

- Reboot: ```sudo reboot```

  ​

