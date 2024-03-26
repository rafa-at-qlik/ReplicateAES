# ReplicateAES 

## Summary
The following repo provides a procedure to encrypt column's data in Qlik Replcate. Let's say we have an Oracle table with credit card information:

<img src="images/oracle_query.png" width="400">

We have the following Replicate task:

<img src="images/replicate_task.png" width="400">

And we want the information to be encrypted before arriving to DataBricks

<img src="images/databricks_enc_query.png" width="400">

But still be able to decrypt the table with the proper key:

<img src="images/databricks_decrypted_query.png" width="400">

## Quick Start

Move to the <INSTALL_DIR>/addons/samples directory and check the contents:

$ cd /opt/attunity/replicate/addons/samples/
$ ls -lrth
total 0
drwxrwxr-x. 2 qlik qlik 148 Feb 20 14:00 MyPasswordProvider
drwxrwxr-x. 2 qlik qlik 148 Feb 20 14:00 MyTransformation
drwxrwxr-x. 2 qlik qlik 131 Feb 20 14:00 docker




