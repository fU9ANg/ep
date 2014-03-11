use epdb;

DROP PROCEDURE IF EXISTS assignment;
DELIMITER //
CREATE PROCEDURE assignment(IN _book_id int, IN _author1_id int, IN _client_type_name1 varchar(64), IN _author2_id int, IN _client_type_name2 varchar(64))
_start:
BEGIN
    DECLARE auid  INT DEFAULT 0;
    DECLARE ctid  INT DEFAULT 0;
    DECLARE coid  INT DEFAULT 0;
    DECLARE _iss  INT DEFAULT 0;
    DECLARE _client1_type_id INT DEFAULT 0;
    DECLARE _client2_type_id INT DEFAULT 0;

    DECLARE rlt   INT DEFAULT 1;

    -- 检查过程的参数的有效性.
    if (_book_id <=> NULL || _author1_id <=> NULL || _client_type_name1 <=> NULL ||
        _author2_id <=> NULL || _client_type_name2 <=> NULL)
    then
        SELECT "[DEBUG]: book_id || author1_id || client_type_name1 = NULL || author2_id = NULL || client_type_name2 = NULL" AS DEBUG_INFO;
        leave _start;
    end if;

    -- 根据客户类型来检查客户id是否有效.
    SELECT count(client_type_id) INTO rlt FROM client_type WHERE client_type_name=_client_type_name1;
    if (rlt = 0) then
        SELECT "[DEBUG]: not found client_type_id in the client_type table." AS DEBUG_INFO;
        leave _start;
    end if;
    SELECT count(client_type_id) INTO rlt FROM client_type WHERE client_type_name=_client_type_name2;
    if (rlt = 0) then
        SELECT "[DEBUG]: not found client_type_id in the client_type table." AS DEBUG_INFO;
        leave _start;
    end if;

    SELECT client_type_id INTO _client1_type_id FROM client_type WHERE client_type_name=_client_type_name1;
    SELECT client_type_id INTO _client2_type_id FROM client_type WHERE client_type_name=_client_type_name2;

    -- 检查此记录是否在author_course表中.
    SELECT count(author_course_id) INTO rlt  FROM author_course
    WHERE author_id=_author1_id AND client_type_id=_client1_type_id AND course_id=_book_id;

    if (rlt = 0) then
        SELECT "[DEBUG]: not found record in the author_course table." AS DEBUG_INFO;
        leave _start;
    end if;

    --
    UPDATE author_course SET author_id=author2_id, client_type_id=_client2_type_id, owner=author2_id, owner_type_id=_client2_type_id
    WHERE author_id=author1_id AND client_type_id=_client1_type_id;

END //
DELIMITER ;
