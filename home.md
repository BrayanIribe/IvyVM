Thanks for getting interested with my project. You can use it by two ways:

## Compiling IvyVM

If you want to experiment with IvyVM or want to do some changes. You can download the repo and open it with Visual Studio 2017 or higher.

```bash
> git clone https://github.com/BrayanIribe/IvyVM
```

Once you cloned the repo, open Visual Studio and open **IvyVM.sln** on it.

## Download binaries

If you don't want to compile the binaries yourself, you can download the public binaries provided in releases.

You can download them from [here](https://github.com/BrayanIribe/IvyVM/releases), search the latest version and download it.

Once you have the executable, go to the next chapter.

## IDE

You can use Visual Studio Code to develop programs for IvyVM.
If you want to get syntax highlighting, you can add this snippet of root of settings.json:
```json
"files.associations": {
        "*.ivy": "cpp",
}
```

## How to use

Spawn a command line window in binaries folder, you can grab one of the scripts of the scripts folder and paste it in the folder. In this example we're going to use **hello-world.ivy**.

```bash
> IvyVM hello-world.ivy
```

The following will show a MessageBox with the Hello World message.

<p align="center">
  <img src="https://user-images.githubusercontent.com/520683/65396276-add8b880-dd59-11e9-93a2-75d397e831f4.png" alt="MessageBox"/>
</p>

If you use the **--verbose** argument you can see instruction execution.

```bash
> IvyVM hello-world.ivy --verbose
```

<p align="center">
  <img src="https://user-images.githubusercontent.com/520683/65396294-d3fe5880-dd59-11e9-8ede-8f91cb1c6b13.png" alt="MessageBox"/>
</p>

The VM printed every instruction in mnemonic.
Feel free to go to the next page!