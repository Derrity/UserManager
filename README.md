使用的数据库是PostgreSQL
请自己配置

数据库结构:
```                                          Table "public.user_info"
    Column    |          Type          | Collation | Nullable |                  Default                   
--------------+------------------------+-----------+----------+--------------------------------------------
 user_id      | integer                |           | not null | nextval('user_info_user_id_seq'::regclass)
 email        | text                   |           | not null |
 username     | character varying(50)  |           | not null |
 password     | character varying(256) |           | not null |
 created_date | date                   |           |          | CURRENT_DATE
 permission   | integer                |           |          | 0
 token        | text                   |           | not null |
 balance      | double precision       |           |          | 0.00
Indexes:
 "user_info_pkey" PRIMARY KEY, btree (user_id)
```