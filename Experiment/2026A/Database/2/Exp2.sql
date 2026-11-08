-- 1. 单表查询：VacantSeats不少于7，按可用名额降序
SELECT MeetingID,
       MeetingTheme,
       VacantSeats
FROM dbo.ReadingMeeting
WHERE VacantSeats >= 7
ORDER BY VacantSeats DESC

-- 2. 多表连接：贺晨阳的已报名业务记录
SELECT lr.ReaderName,
       rm.MeetingTheme,
       rr.GuestNumber,
       rr.ReserveOn
FROM dbo.ReadingReservation rr
JOIN dbo.LibraryReader lr
  ON rr.ReaderID = lr.ReaderID
JOIN dbo.ReadingMeeting rm
  ON rr.MeetingID = rm.MeetingID
WHERE lr.ReaderName = '贺晨阳'
  AND rr.ReservationStatus = '已报名';

-- 3. LEFT JOIN + GROUP BY：全部项目的已报名记录条数与GuestNumber合计，B006也要输出0
SELECT rm.MeetingID,
       rm.MeetingTheme,
       COUNT(rr.ReservationID) AS ValidRecordCount,
       ISNULL(SUM(rr.GuestNumber), 0) AS TotalGuestNumber
FROM dbo.ReadingMeeting rm
LEFT JOIN dbo.ReadingReservation rr
  ON rm.MeetingID = rr.MeetingID
 AND rr.ReservationStatus = '已报名'
GROUP BY rm.MeetingID,
         rm.MeetingTheme;

-- 4. 按MeetingKind分组统计已报名GuestNumber之和，仅显示合计不少于4的类别，并按合计降序
SELECT rm.MeetingKind,
       SUM(rr.GuestNumber) AS TotalGuestNumber
FROM dbo.ReadingReservation rr
JOIN dbo.ReadingMeeting rm
  ON rr.MeetingID = rm.MeetingID
WHERE rr.ReservationStatus = '已报名'
GROUP BY rm.MeetingKind
HAVING SUM(rr.GuestNumber) >= 4
ORDER BY TotalGuestNumber DESC;

-- 5. 查询“中文系”人员的已报名业务记录
SELECT lr.ReaderName,
       lr.ReaderGroup,
       rm.MeetingTheme,
       rm.MeetingKind
FROM dbo.ReadingReservation rr
JOIN dbo.LibraryReader lr
  ON rr.ReaderID = lr.ReaderID
JOIN dbo.ReadingMeeting rm
  ON rr.MeetingID = rm.MeetingID
WHERE lr.ReaderGroup = '中文系'
  AND rr.ReservationStatus = '已报名';

-- 6. IN子查询：已报名且参与过“科学史阅读分享”的人员编号和姓名（去重）
SELECT DISTINCT lr.ReaderID,
                lr.ReaderName
FROM dbo.LibraryReader lr
WHERE lr.ReaderID IN (
    SELECT rr.ReaderID
    FROM dbo.ReadingReservation rr
    JOIN dbo.ReadingMeeting rm
      ON rr.MeetingID = rm.MeetingID
    WHERE rr.ReservationStatus = '已报名'
      AND rm.MeetingTheme = '科学史阅读分享'
);

-- 7. 相关EXISTS子查询：至少有一条已报名业务记录的人员
SELECT lr.ReaderID,
       lr.ReaderName
FROM dbo.LibraryReader lr
WHERE EXISTS (
    SELECT 1
    FROM dbo.ReadingReservation rr
    WHERE rr.ReaderID = lr.ReaderID
      AND rr.ReservationStatus = '已报名'
);

-- 8a. NOT EXISTS：从未有已报名记录的人员，结果应包含R006
SELECT lr.ReaderID,
       lr.ReaderName
FROM dbo.LibraryReader lr
WHERE NOT EXISTS (
    SELECT 1
    FROM dbo.ReadingReservation rr
    WHERE rr.ReaderID = lr.ReaderID
      AND rr.ReservationStatus = '已报名'
);

-- 8b. NOT EXISTS：从未有已报名记录的项目，结果应包含B006
SELECT rm.MeetingID,
       rm.MeetingTheme
FROM dbo.ReadingMeeting rm
WHERE NOT EXISTS (
    SELECT 1
    FROM dbo.ReadingReservation rr
    WHERE rr.MeetingID = rm.MeetingID
      AND rr.ReservationStatus = '已报名'
);

-- 9. 双重NOT EXISTS：对全部类别为“基础”的项目均有已报名记录的人员，应包含R001
SELECT lr.ReaderID,
       lr.ReaderName
FROM dbo.LibraryReader lr
WHERE NOT EXISTS (
    SELECT 1
    FROM dbo.ReadingMeeting rm
    WHERE rm.MeetingKind = '基础'
      AND NOT EXISTS (
          SELECT 1
          FROM dbo.ReadingReservation rr
          WHERE rr.ReaderID = lr.ReaderID
            AND rr.MeetingID = rm.MeetingID
            AND rr.ReservationStatus = '已报名'
      )
);

-- 10a. UNION：合并参与过B001或B002的人员编号
SELECT ReaderID
FROM dbo.ReadingReservation
WHERE MeetingID = 'B001'
  AND ReservationStatus = '已报名'
UNION
SELECT ReaderID
FROM dbo.ReadingReservation
WHERE MeetingID = 'B002'
  AND ReservationStatus = '已报名';

-- 10b. EXCEPT：尚无任何已报名业务记录的人员编号
SELECT ReaderID
FROM dbo.LibraryReader
EXCEPT
SELECT ReaderID
FROM dbo.ReadingReservation
WHERE ReservationStatus = '已报名';