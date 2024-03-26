## ReplicateAES 

# Summary
The following repo provides a procedure to encrypt column's data in Qlik Replcate. Let's say we have an Oracle table with credit card information:

<img src="images/oracle_query.png" width="400">

We have the following Replicate task:

<img src="images/replicate_task.png" width="400">

And we want the information to be encrypted before arriving to DataBricks

<img src="images/databricks_enc_query.png" width="400">

But still be able to decrypt the table with the proper key:

<img src="images/databricks_decrypted_query.png" width="400">


