/*
SET @_book_id=4;
SET @_author_id=1;
SET @_client_type_name="教师";

show procedure status;
*/

use epdb;

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

-- CALL sp_publish(@_book_id, @_author_id, @_client_type_name);
