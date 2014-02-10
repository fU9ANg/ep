/*
SET @_in=1;
show procedure status;
*/

use epdb;
DROP PROCEDURE IF EXISTS sp_test;
DELIMITER //
CREATE PROCEDURE sp_test(IN _in int)
_exec:
BEGIN
/*
    if (_in <=> NULL || _out <=> NULL) then
        SELECT "parameters is NULL";
        leave _exec;
    end if;
*/
    if (_in = 0) then
        SELECT 0 AS rlt, "等于0" AS msg;
    elseif (_in < 0) then
        SELECT -1 AS rlt, "小于0" AS msg;
    elseif (_in > 0) then
        SELECT 1 AS rlt, "大于1" AS msg;
    end if;
END //

DELIMITER ;

CALL sp_test(@_in);
