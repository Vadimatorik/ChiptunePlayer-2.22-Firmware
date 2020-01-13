fat_fs_err = {}
fat_fs_err[-1] = "FR_DISK_ERR"                -- A hard error occurred in the low level disk I/O layer
fat_fs_err[-2] = "FR_INT_ERR"                 -- Assertion failed
fat_fs_err[-3] = "FR_NOT_READY"               -- The physical drive cannot work
fat_fs_err[-4] = "FR_NO_FILE"                 -- Could not find the file
fat_fs_err[-5] = "FR_NO_PATH"                 -- Could not find the path
fat_fs_err[-6] = "FR_INVALID_NAME"            --  The path name format is invalid
fat_fs_err[-7] = "FR_DENIED"                  -- Access denied due to prohibited access or directory full
fat_fs_err[-8] = "FR_EXIST"                   -- Access denied due to prohibited access
fat_fs_err[-9] = "FR_INVALID_OBJECT"          -- The file/directory object is invalid
fat_fs_err[-10] = "FR_WRITE_PROTECTED"        -- The physical drive is write protected
fat_fs_err[-11] = "FR_INVALID_DRIVE"          -- The logical drive number is invalid
fat_fs_err[-12] = "FR_NOT_ENABLED"            -- The volume has no work area
fat_fs_err[-13] = "FR_NO_FILESYSTEM"          -- There is no valid FAT volume
fat_fs_err[-14] = "FR_MKFS_ABORTED"           -- The f_mkfs() aborted due to any problem
fat_fs_err[-15] = "FR_TIMEOUT"                -- Could not get a grant to access the volume within defined period
fat_fs_err[-16] = "FR_LOCKED"                 -- The operation is rejected according to the file sharing policy
fat_fs_err[-17] = "FR_NOT_ENOUGH_CORE"        -- LFN working buffer could not be allocated
fat_fs_err[-18] = "FR_TOO_MANY_OPEN_FILES"    -- Number of open files > FF_FS_LOCK
fat_fs_err[-19] = "FR_INVALID_PARAMETER"      -- Given parameter is invalid

fat_fs_err[-20] = "OS_MALLOC_FAIL"            -- malloc fail.

function get_fat_fs_err_str (code)
	return fat_fs_err[code]
end