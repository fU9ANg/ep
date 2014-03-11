use epdb;

DROP PROCEDURE IF EXISTS storeup;
DELIMITER //
CREATE PROCEDURE storeup(IN _book_id int, IN _author_id int, IN _client_type_name varchar(64))
_start:
BEGIN
    DECLARE auid  INT DEFAULT 0;
    DECLARE ctid  INT DEFAULT 0;
    DECLARE coid  INT DEFAULT 0;
    DECLARE _iss  INT DEFAULT 0;
    DECLARE _client_type_id INT DEFAULT 0;

    DECLARE rlt   INT DEFAULT 1;

    -- 检查过程的参数的有效性.
    if (_book_id <=> NULL || _author_id <=> NULL || _client_type_name <=> NULL)
    then
        SELECT "[DEBUG]: book_id || author_id || client_type_name = NULL" AS DEBUG_INFO;
        leave _start;
    end if;

    -- 根据客户类型来检查客户id是否有效.
    SELECT count(client_type_id) INTO rlt FROM client_type WHERE client_type_name=_client_type_name;
    if (rlt = 0) then
        SELECT "[DEBUG]: not found client_type_id in the client_type table." AS DEBUG_INFO;
        leave _start;
    end if;

    SELECT client_type_id INTO _client_type_id FROM client_type WHERE client_type_name=_client_type_name;

    -- 检查此记录是否在author_course表中.
    SELECT count(author_course_id) INTO rlt  FROM author_course WHERE course_id=_book_id;

    if (rlt = 0) then
        SELECT "[DEBUG]: not found record in the author_course table." AS DEBUG_INFO;
        leave _start;
    end if;

    SELECT author_id INTO auid FROM author_course WHERE course_id=_book_id AND _iss=1 AND flag=0;
    SELECT course_id INTO coid FROM author_course WHERE course_id=_book_id AND _iss=1 AND flag=0;
    SELECT client_type_id INTO ctid FROM author_course WHERE course_id=_book_id AND _iss=1 AND flag=0;

    INSERT INTO author_course(author_id, client_type_id, course_id, issell, owner, owner_type_id, flag)
    VALUE (auid, ctid, coid, 0, _author_id, _client_type_id, 2);

END //
DELIMITER ;
