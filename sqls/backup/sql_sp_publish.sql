/*
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

show procedure status;
*/

use epdb;

DROP PROCEDURE IF EXISTS sql_sp_publish;
DELIMITER //
CREATE PROCEDURE sql_sp_publish(IN _group_id int, IN _course_name varchar(200), IN _time varchar(200), IN _belongs int, IN _art int, IN _lang int, IN _comm int, IN _health int, IN _science int, IN _resPath varchar(200), IN _client_id int, IN _client_type int)
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

-- CALL sql_sp_publish(@_group_id, @_course_name, @_time, @_belongs, @_art, @_lang, @_comm, @_health, @_science, @_resPath, @_client_id, @_client_type);
