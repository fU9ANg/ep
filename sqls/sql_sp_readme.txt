sql_sp_publish:
    1. insert data to `course`.
    2. insert data to `grade_course`.
    3. insert data to `author_course`.

sql_sp_sell:
    1. update data from `author_course` set issell -> 1.

sql_sp_largess:
    if (the course of author == xxxx)
        insert data to `author_course` author=author and owner=xxxx.
    else
        update data from `author_course` owner=xxxx.

sql_sp_assignment:
    update data from `author_course` set author=xxxx and set owner=xxxx.

sql_sp_purchase:
    insert data to `author_course` set owner=xxxx and set flag=1.

sql_sp_storeup:
    insert data to `author_course` set owner=xxxx and set flgs=0.
