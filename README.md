![image](https://github.com/rafa-at-qlik/ReplicateAES/assets/124567817/6cc1964f-a69c-4d61-a226-6c2a89060066)# ReplicateAES 

## Summary
The following repo provides a procedure to encrypt column's data in Qlik Replcate. Let's say we have an Oracle table with credit card information:

<img src="images/oracle_query.png" width="600">

We have the following Replicate task:

<img src="images/replicate_task.png" width="600">

And we want the information to be encrypted before arriving to DataBricks

<img src="images/databricks_enc_query.png" width="600">

But still be able to decrypt the table with the proper key:

<img src="images/databricks_decrypted_query.png" width="600">

## Quick Start

Move to the <INSTALL_DIR>/addons/samples directory and check the contents:

```console
$ cd /opt/attunity/replicate/addons/samples/
$ ls -lrth
total 0
drwxrwxr-x. 2 qlik qlik 148 Feb 20 14:00 MyPasswordProvider
drwxrwxr-x. 2 qlik qlik 148 Feb 20 14:00 MyTransformation
drwxrwxr-x. 2 qlik qlik 131 Feb 20 14:00 docker
```

MyTransformation is the User-defined Transformation template that we include in our product. Let’s copy the entire directory to implement aes_encrypt():

```console
$ cd /opt/attunity/replicate/addons/samples
$ cp -Rp MyTransformation aes_encrypt
```

Copy the types defined in ar_addon.h and ar_addon_transformation.h to the following path:

```console
$ cd /opt/attunity/replicate/addons/include
$ cp *.h /opt/attunity/replicate/addons/samples/aes_encrypt/.
```

Let’s rename MyTransformation.c and clean up the rest of MyTransformation files:

```console
$ cd /opt/attunity/replicate/addons/samples/aes_encrypt
$ mv MyTransformation.c aes_encrypt.c 
$ rm MyTransformation*
$ ll
total 20
-rwxrwxr-x. 1 qlik qlik 7014 Feb 26 11:42 ar_addon.h
-rwxrwxr-x. 1 qlik qlik 2650 Feb 26 11:43 ar_addon_transformation.h
-rwxrwxr-x. 1 qlik qlik 2446 Jan 14 00:29 aes_encrypt.c
-rwxrwxr-x. 1 qlik qlik  543 Jan 14 00:29 Makefile
```

Upload the contents of aes_encrypt.c from this repository or replace the file (ensure you maintain the same permissions).

We include a Makefile that simplifies the compilation but we need replace the MyTrasformation string with the new UDT name:

```console
$ cd /opt/attunity/replicate/addons/samples/aes_encrypt
$ grep -i MyTransformation Makefile
TARGET_LIB = MyTransformation.so # target lib
SRCS = MyTransformation.c # source files
        cp ./MyTransformation.so ../../
$ sed -i 's/MyTransformation/aes_encrypt/g' Makefile
$ grep -i MyTransformation Makefile
$
```
