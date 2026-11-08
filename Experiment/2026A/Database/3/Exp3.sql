-- 1
INSERT INTO dbo.LibraryReader (ReaderID, ReaderName, ReaderGroup, ReadingInterest)
VALUES ('R007', N'苏铭泽-测试', N'测试部门', N'历史');

SELECT * FROM dbo.LibraryReader WHERE ReaderID = 'R007';

-- 2
INSERT INTO dbo.ReadingReservation (ReservationID, ReaderID, MeetingID, GuestNumber, ReserveOn, ReservationStatus)
VALUES ('Q0012', 'R007', 'B005', 1, '2026-09-20', N'已报名');

UPDATE dbo.ReadingMeeting
SET VacantSeats -= 1
WHERE MeetingID = 'B005'

SELECT MeetingID, VacantSeats FROM dbo.ReadingMeeting WHERE MeetingID = 'B005';

-- 3
UPDATE dbo.LibraryReader
SET ReaderName = N'柳清越-已核验'
WHERE ReaderID = 'R001';

UPDATE dbo.ReadingReservation
SET GuestNumber = 2
WHERE ReservationID = 'Q0012';

UPDATE dbo.ReadingMeeting
SET VacantSeats = VacantSeats - 1
WHERE MeetingID = 'B005';

SELECT rr.ReservationID, rr.GuestNumber, m.VacantSeats
FROM dbo.ReadingReservation rr
JOIN dbo.ReadingMeeting m ON rr.MeetingID = m.MeetingID
WHERE rr.ReservationID = 'Q0012';
GO

-- 4
CREATE VIEW dbo.vw_ReadingMeeting_Summary AS
SELECT m.MeetingID,
       m.MeetingTheme,
       m.MeetingKind,
       COUNT(r.ReservationID)            AS ValidRecordCount,
       ISNULL(SUM(r.GuestNumber), 0)     AS TotalGuestNumber
FROM dbo.ReadingMeeting m
LEFT JOIN dbo.ReadingReservation r
       ON m.MeetingID = r.MeetingID
      AND r.ReservationStatus = N'已报名'
GROUP BY m.MeetingID, m.MeetingTheme, m.MeetingKind;
GO

SELECT * FROM dbo.vw_ReadingMeeting_Summary;

-- 5
UPDATE dbo.ReadingReservation
SET ReservationStatus = N'已取消'
WHERE ReservationID = 'Q0011';

UPDATE dbo.ReadingMeeting
SET VacantSeats += 1
WHERE MeetingID = 'B001'

SELECT * FROM dbo.vw_ReadingMeeting_Summary
WHERE MeetingID = 'B001';

-- 6
SELECT * FROM dbo.vw_ReadingMeeting_Summary
WHERE TotalGuestNumber >= 3
ORDER BY TotalGuestNumber DESC;

-- 7
UPDATE dbo.ReadingMeeting
SET VacantSeats = VacantSeats + 2
WHERE MeetingID = 'B005';

DELETE FROM dbo.ReadingReservation
WHERE ReservationID = 'Q0012';

DELETE FROM dbo.LibraryReader
WHERE ReaderID = 'R007';


SELECT * FROM dbo.LibraryReader
WHERE ReaderID = 'R007';
SELECT * FROM dbo.ReadingReservation
WHERE ReservationID = 'Q0012';