/*
SET @_book_id=4;
SET @_author_id=1;
SET @_client_type_id=2;
SET @_account="t2";

show procedure status;
*/

use epdb;

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
    DECLARE _ownerid INT DEFAULT 0;

    DECLARE rlt   INT DEFAULT 0;

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
        SELECT count(student_id) INTO rlt FROM student WHERE account=_account;
        if (rlt = 0) then
            SELECT "[Error]: not found account in the 'student' or 'teacher' table.";
            leave _exec;
        else
            SET _ct=4;
        end if;
    else
        SET _ct=2;
    end if;

    -- check book id
    SELECT count(course_id) INTO rlt FROM course WHERE course_id=_book_id;
    if (rlt = 0) then
        SELECT "[Error]: not found book_id in the course table.";
        leave _exec;
    end if;

    -- check pararmeters in the author_course.
    SELECT count(author_course_id) INTO rlt FROM author_course 
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
        SELECT student_id INTO _ownerid FROM student WHERE account=_account;
        -- check is exist record.
        SELECT count(author_course_id) INTO rlt FROM author_course 
        WHERE course_id=_book_id AND author_id=_client1_id 
        AND client_type_id=_client1_type AND owner=_ownerid AND owner_type_id=4;
        if (rlt = 0) then
            SELECT "[Error]: exist owner is student record in the author_course table.";
            leave _exec;
        end if;
        INSERT INTO author_course(author_id, client_type_id, course_id, ispublish, owner, owner_type_id)
        VALUE(auid, ctid, coid, ispub, _ownerid, 4);
        SELECT "[INFO]: success - transfer to student.";
    elseif (_ct = 2) then
        SELECT teacher_id INTO _ownerid FROM teacher WHERE account=_account;
        -- check is exist record.
        SELECT count(author_course_id) INTO rlt FROM author_course 
        WHERE course_id=_book_id AND author_id=_client1_id 
        AND client_type_id=_client1_type AND owner=_ownerid AND owner_type_id=2;
        if (rlt = 0) then
            SELECT "[Error]: exist owner is teacher record in the author_course table.";
            leave _exec;
        end if;
        INSERT INTO author_course(author_id, client_type_id, course_id, ispublish, owner, owner_type_id)
        VALUE(auid, ctid, coid, ispub, _ownerid, 2);
        SELECT "[INFO]: success - transfer to teacher.";

    end if;
END //

DELIMITER ;

-- CALL sp_transfer(@_book_id, @_author_id, @_client_type_id, @_account);
