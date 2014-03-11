use epdb;

/*
SET @group_id=1;
SET @course_type=3;
SET @course_name="89909绘图22989822";
SET @time="2014/1/1";
SET @belongs=7;
SET @art=1;
SET @lang=1;
SET @comm=1;
SET @health=1;
SET @science=1;
SET @resPath="/skdjfk/sdkfj/";
SET @client_id=80;
SET @client_type=2;
CALL sql_sp_publish(@group_id, @course_type, @course_name, @time, @belongs, @art, @lang, @comm, @health, @science, @resPath, @client_id, @client_type);
*/

/*
 * 出售 (publish)
 */

DROP PROCEDURE IF EXISTS sql_sp_publish;
DELIMITER //

CREATE PROCEDURE sql_sp_publish(IN _group_id int, IN _course_name varchar(200), IN _time varchar(200), IN _belongs int, IN _art int, IN _lang int, IN _comm int, IN _health int, IN _science int, IN _resPath varchar(200), IN _client_id int, IN _client_type int, IN _course_type int)
_start:
BEGIN
    DECLARE cid     INT DEFAULT 1;
    DECLARE gid     INT DEFAULT 1;
    DECLARE _result INT DEFAULT 1;
    DECLARE _idx    INT DEFAULT 0;
    DECLARE _min    INT DEFAULT 0;
    DECLARE _max    INT DEFAULT 0;

    -- 检查过程的参数的有效性.
    if (_group_id <=> NULL ||
        _course_type <=> NULL ||
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
        SELECT "[DEBUG]: the parameter format is error" AS DEBUG_INFO;
        leave _start;
    end if;

    -- 课程表中不能有相同的课程名称.
    SELECT count(course_id) INTO _result FROM course WHERE course_name=_course_name;
    if (_result > 0) then
        SELECT "[DEBUG]: have same course_name in the course table" AS DEBUG_INFO;
        leave _start;
    else

        -- 检查是否是有效的client_id, 在client_type表中.
        SELECT max(client_type_id) INTO _max FROM client_type;
        SELECT min(client_type_id) INTO _min FROM client_type;
        if (_client_type > _max || _client_type < _min) then
            SELECT "[DEBUG]: the parameter client_type is error!" AS DEBUG_INFO;
            leave _start;
        end if;

        -- 只支持client_type为教师或学生.
        if (!(_client_type=2 OR _client_type=4)) then
            SELECT "[DEBUG]: only support author is teacher or student." AS DEBUG_INFO;
            leave _start;
        end if;

        -- 如果client_type为学生, 检查有效的学生id号.
        if (_client_type = 2) then
            SELECT max(teacher_id) INTO _max FROM teacher;
            SELECT min(teacher_id) INTO _min FROM teacher;
            if (_client_id > _max || _client_id < _min) then
                SELECT "[DEBUG]: the parameter client_id of teacher is error!" AS DEBUG_INFO;
                leave _start;
            end if;
        end if;

        -- 如果client_type为教师, 检查有效的教师id号.
        if (_client_type = 4) then
            SELECT max(student_id) INTO _max FROM student;
            SELECT min(student_id) INTO _min FROM student;
            if (_client_id > _max || _client_id < _min) then
                SELECT "[DEBUG]: the parameter client_id of student is error!" AS DEBUG_INFO;
                leave _start;
            end if;
        end if;

        -- 添加课程.
        INSERT INTO course(course_name,group_id,course_type_id,language,art,community,health,science,course_date,resPath)
        VALUES(_course_name, _group_id, _course_type, _lang, _art, _comm, _health, _science, _time, _resPath);

        -- 遍历插入记录到年级课程表.
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


        -- 插入记录到author_course表.
        INSERT INTO author_course(author_id, client_type_id, course_id, issell, owner)
        VALUE (_client_id, _client_type, cid, 0, _client_id);

    end if;
END //
DELIMITER ;
