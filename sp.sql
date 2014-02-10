UPLoad:

----
    Driver *pDriver = mysql::get_mysql_driver_instance();  
    try  
    {  
        auto_ptr<Connection> pConnection(pDriver->connect("tcp://127.0.0.1:3306", "root", "123456"));  
        auto_ptr<PreparedStatement>prepQuery(pConnection->prepareStatement(  
            "call searchserver.query(?, @sid, @sname)" ));  
        prepQuery->setString(1, "ABC");  
        prepQuery->execute();  
      
        auto_ptr<PreparedStatement>prepSelect(pConnection->prepareStatement(  
            "select @sid as sid, @sname as sname"));  
        auto_ptr<ResultSet>lpRes(prepSelect->executeQuery());  
        int nRows = lpRes->rowsCount();  
        while(lpRes->next())  
        {  
            uint64_t ID = lpRes->getUInt64(1);  
            string strName = lpRes->getString(2);  
        }  
    }  
    catch (SQLException& e)  
    {  
        string str = e.what();  
        return;  
    }  
----



SET @_group_id=1;
SET @_course_name="绘小猴子111";
SET @_time="2014/1/14";
SET @_grade_name="小班";
SET @_art=23;
SET @_lang=45;
SET @_comm=74;
SET @_health=83;
SET @_science=34;
SET @_resPath="./resPath";
SET @_out=0;

DROP PROCEDURE IF EXISTS proc1;

CALL proc1(@_group_id, @_course_name, @_time, @_grade_name, @_art, @_lang, @_comm, @_health, @_science, @_resPath, @_out);


DELIMITER //
CREATE PROCEDURE proc1(IN _group_id int, IN _course_name varchar(200), IN _time varchar(200), IN _grade_name varchar(200), IN _art int, IN _lang int, IN _comm int, IN _health int, IN _science int, IN _resPath varchar(200), OUT _rlt int)
_exec:
BEGIN
    DECLARE cid     INT DEFAULT 1;
    DECLARE gid     INT DEFAULT 1;
    DECLARE _result INT DEFAULT 1;

    SELECT count(course_id) INTO _result FROM course WHERE course_name=_course_name;
    if _result > 0 then
        SET _rlt = 0;
        leave _exec;
    else
        INSERT INTO course(course_name,group_id,language,art,community,health,science,course_date,resPath)
        VALUES(_course_name, _group_id, _lang, _art, _comm, _health, _science, _time, _resPath);

        SELECT course_id INTO cid FROM course WHERE course_name=_course_name;

        SELECT grade_id INTO gid FROM grade WHERE grade_name=_grade_name;
        INSERT INTO grade_course(grade_id, course_id) VALUE (gid, cid);
        SET _rlt = 1;
    end if;
END //

DELIMITER ;
==================================UPLOAD=========================================================
SET @_group_id=1;
SET @_course_name="绘小猴子3";
SET @_time="2014/1/14";
SET @_grade_name="小班";
SET @_art=23;
SET @_lang=45;
SET @_comm=74;
SET @_health=83;
SET @_science=34;
SET @_resPath="./resPath";
SET @_belongs=7;
SET @_out=0;
SET @_client_id=2;
SET @_client_type=2;

DROP PROCEDURE IF EXISTS sp_upload;
DELIMITER //
CREATE PROCEDURE sp_upload(IN _group_id int, IN _course_name varchar(200), IN _time varchar(200), IN _belongs int, IN _art int, IN _lang int, IN _comm int, IN _health int, IN _science int, IN _resPath varchar(200), IN _client_id int, IN _client_type int)
_exec:
BEGIN
    DECLARE cid     INT DEFAULT 1;
    DECLARE gid     INT DEFAULT 1;
    DECLARE _result INT DEFAULT 1;
    DECLARE _idx    INT DEFAULT 0;

    if (_group_id <=> NULL || 
        _course_name <=> NULL || 
        _time <=> NULL || 
        _belongs <=> NULL ||
        _art <=> NULL ||
        _lang <=> NULL ||
        _comm <=> NULL ||
        _health <=> NULL ||
        _science <=> NULL ||
        _resPath <=> NULL ||
        _client_id <=> NULL ||
        _client_type <=> NULL)
    then
        SELECT "HERERER....";
        leave _exec;
    end if;

    SELECT count(course_id) INTO _result FROM course WHERE course_name=_course_name;
    if _result > 0 then
        leave _exec;
    else
        INSERT INTO course(course_name,group_id,language,art,community,health,science,course_date,resPath)
        VALUES(_course_name, _group_id, _lang, _art, _comm, _health, _science, _time, _resPath);

        SELECT course_id INTO cid FROM course WHERE course_name=_course_name;

        select _belongs;
        WHILE (_idx < 32) DO 
            if ((_belongs & (1<<_idx)) != 0) then
                SELECT _belongs & (1<<_idx) AS __HH;
                SET _idx = _idx + 1;
                INSERT INTO grade_course(grade_id, course_id) VALUE (_idx, cid);
            else
                SET _idx = _idx + 1;
            end if;
        END WHILE;

        SELECT count(teacher_id) INTO _result FROM teacher;
        if (_result < _client_id) then
            leave _exec;
        end if;
        SELECT count(client_type_id) INTO _result FROM client_type;
        if (_result < _client_type) then
            leave _exec;
        end if;

        INSERT INTO author_course(author_id, client_type_id, course_id, ispublish)
        VALUE (_client_id, _client_type, cid, 0);

    end if;
END //

DELIMITER ;

CALL proc2(@_group_id, @_course_name, @_time, @_belongs, @_art, @_lang, @_comm, @_health, @_science, @_resPath, @_client_id, @_client_type);


==================================PUBLISH=========================================================
SET @_book_id=4;
SET @_author_id=1;
SET @_client_type_name="教师";

DROP PROCEDURE IF EXISTS sp_publish;
DELIMITER //
CREATE PROCEDURE sp_publish(IN _book_id int, IN _author_id int, IN _client_type_name varchar(64))
_exec:
BEGIN
    DECLARE auid  INT DEFAULT 0;
    DECLARE ctid  INT DEFAULT 0;
    DECLARE coid  INT DEFAULT 0;
    DECLARE ispub INT DEFAULT 0;
    DECLARE _client_type_id INT DEFAULT 0;

    DECLARE rlt   INT DEFAULT 1;

    if (_book_id <=> NULL || _author_id <=> NULL || _client_type_name <=> NULL) then
        SELECT "[Error]: book_id || author_id || client_type_name = NULL";
        leave _exec;
    end if;

    SELECT count(course_id) INTO rlt FROM author_course WHERE course_id=_book_id;
    if (rlt = 0) then
        SELECT "[Error]: not found book_id in the author_course table.";
        leave _exec;
    else
        SELECT count(client_type_id) INTO rlt FROM client_type WHERE client_type_name=_client_type_name;
        if (rlt = 0) then
            SELECT "[Error]: not found client_type_id in the client_type table.";
            leave _exec;
        else
            SELECT client_type_id INTO _client_type_id FROM client_type WHERE client_type_name=_client_type_name;
            SELECT count(teacher_id) INTO rlt FROM teacher WHERE teacher_id=_author_id;
            if (rlt = 0) then
                SELECT "[Error]: not found author_id in the author table.";
                leave _exec;
            else

                SELECT count(author_course_id) INTO rlt  FROM author_course 
                WHERE author_id=_author_id AND client_type_id=_client_type_id AND course_id=_book_id;

                if (rlt = 0) then
                    SELECT "[Error]: not found record in the author_course table.";
                    leave _exec;
                end if;

                SELECT ispublish INTO ispub FROM author_course 
                WHERE author_id=_author_id AND client_type_id=_client_type_id AND course_id=_book_id;

                SELECT author_id INTO auid  FROM author_course 
                WHERE author_id=_author_id AND client_type_id=_client_type_id AND course_id=_book_id;

                SELECT course_id INTO coid  FROM author_course 
                WHERE author_id=_author_id AND client_type_id=_client_type_id AND course_id=_book_id;

                SELECT client_type_id INTO ctid  FROM author_course 
                WHERE author_id=_author_id AND client_type_id=_client_type_id AND course_id=_book_id;

                if (ispub = 1) then
                    leave _exec;
                else
                    INSERT INTO author_course(author_id, client_type_id, course_id, ispublish)
                    VALUE(auid, ctid, coid, 1);
                    -- UPDATE author_course SET ispublish=1 WHERE course_id=_book_id;
                end if;
            end if;
        end if;
    end if;
END //

DELIMITER ;

CALL sp_publish(@_book_id, @_author_id, @_client_type_name);

==================================TRANSFER=========================================================
SET @_book_id=4;
SET @_author_id=1;
SET @_client_type_name="教师";

DROP PROCEDURE IF EXISTS sp_transfer;
DELIMITER //
CREATE PROCEDURE sp_transfer(IN _book_id int, IN _client1_id int, IN _client1_type INT, IN _account varchar (64))
_exec:
BEGIN
    DECLARE auid  INT DEFAULT 0;
    DECLARE ctid  INT DEFAULT 0;
    DECLARE coid  INT DEFAULT 0;
    DECLARE ispub INT DEFAULT 0;
    DECLARE _client_type_id INT DEFAULT 0;

    DECLARE _ct INT DEFAULT 0;
    DECLARE _owner INT DEFAULT 0;

    DECLARE rlt   INT DEFAULT 1;

    if (_book_id <=> NULL || _client1_id <=> NULL || _client1_type <=> NULL || _account <=> NULL) then
        SELECT "[Error]: book_id || author_id || client_type_name || account = NULL";
        leave _exec;
    end if;

    -- check client type
    SELECT count(client_type_id) INTO rlt FROM client_type;
    if (rlt < _client1_type) then
        SELECT "[Error]: client1_type!...";
        leave _exec;
    end if;

    -- check client id
    if (_client1_type = 2) then
        SELECT count(teacher_id) INTO rlt FROM teacher;
        if (rlt < _client1_id) then
            SELECT "[Error]: client1_id in the teacher table.";
            leave _exec;
        end if;
    elseif (_client1_type = 4) then
        SELECT count(student_id) INTO rlt FROM student;
        if (rlt < _client1_id) then
            SELECT "[Error]: client1_id in the student table.";
            leave _exec;
        end if;
    else
        SELECT "[Error]: client1_id!!!.";
        leave _exec;
    end if;

    -- check account
    SELECT count(teacher_id) INTO rlt FROM teacher WHERE account=_account;
    if (rlt = 0) then
        SELECT count(student_id) INTO rlt FROM student WEHRE account=_account;
        if (rlt = 0) then
            SELECT "[Error]: not found account in the 'student' or 'teacher' table.";
            leave _exec;
        else
            SET _ct = 4;
        end if;
    else
        SET _ct = 2;
    end if;

    -- check book id
    SELECT count(course_id) INTO rlt FROM course WHERE course_id=_book_id;
    if (rlt = 0) then
        SELECT "[Error]: not found book_id in the course table.";
        leave _exec;
    end if;

    -- check pararmeters in the author_course.
    SELECT count(author_course_id) AS rlt FROM author_course 
    WHERE course_id=_book_id AND author_id=_client1_id AND client_type_id=_client1_type;
    if (rlt = 0) then
        SELECT "[Error]: not found data in the author_course table.";
        leave _exec;
    end if;

    -- TODO
    SELECT ispublish INTO ispub FROM author_course 
    WHERE author_id=_client1_id AND client_type_id=_client1_type AND course_id=_book_id;

    SELECT author_id INTO auid  FROM author_course 
    WHERE author_id=_client1_id AND client_type_id=_client1_type AND course_id=_book_id;

    SELECT course_id INTO coid  FROM author_course 
    WHERE author_id=_client1_id AND client_type_id=_client1_type AND course_id=_book_id;

    SELECT client_type_id INTO ctid  FROM author_course 
    WHERE author_id=_client1_id AND client_type_id=_client1_type AND course_id=_book_id;

    SELECT ispublish INTO ispub  FROM author_course 
    WHERE author_id=_client1_id AND client_type_id=_client1_type AND course_id=_book_id;

    if (_ct = 4) then
        SELECT student_id AS _ownerid FROM student WHERE account=_account;
        INSERT INTO author_course(author_id, client_type_id, course_id, ispublish, owner)
        VALUE(auid, ctid, coid, ispub, _ownerid);
    else if (_ct = 2) then
        SELECT teacher_id AS _ownerid FROM teacher WHERE account=_account;
        INSERT INTO author_course(author_id, client_type_id, course_id, ispublish, owner)
        VALUE(auid, ctid, coid, ispub, _ownerid);
    end if;
END //

DELIMITER ;

CALL sp_publish(@_book_id, @_author_id, @_client_type_name);
