## ReplicateAES 

# Summary
The following repo provides a procedure to encrypt column's data in Qlik Replcate. Let's say we have an Oracle table with credit card information:

<img src="images/oracle_query.png" width="1000">

We have the following replicate task:

![alt text](images/replicate_task.png)

And we want the information to be encrypted before arriving to DataBricks

![alt text](images/databricks_enc_query.png)

But still be able to decrypt the table with the proper key:

![alt text](images/databricks_decrypted_query.png)

