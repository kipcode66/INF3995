PRAGMA foreign_keys=OFF;
BEGIN TRANSACTION;
CREATE TABLE user (ip VARCHAR(16), mac VARCHAR(18) UNIQUE, name VARCHAR(256), user_id INTEGER UNIQUE, is_blacklisted INTEGER);
CREATE TABLE userConnection (user_id INTEGER UNIQUE NOT NULL PRIMARY KEY, connection_expiration REAL, FOREIGN KEY(user_id) REFERENCES user(user_id));
CREATE TABLE adminLogin (login VARCHAR(256) UNIQUE NOT NULL, hashed_password VARCHAR(32) NOT NULL, salt VARCHAR(32) NOT NULL);
CREATE TABLE adminConnection (login VARCHAR(256) UNIQUE NOT NULL, isConnected INTEGER, admin_id INTEGER, timeStamp REAL);
CREATE TABLE songs (title VARCHAR(256), artist VARCHAR(256), user_id INTEGER NOT NULL, duration INTEGER, path VARCHAR(256), song_order INTEGER, deleted_by_admin INTEGER, was_played INTEGER, timestamp REAL);
CREATE TRIGGER IF NOT EXISTS add_order_auto_increment AFTER INSERT ON songs
BEGIN UPDATE songs SET song_order = NEW.rowid WHERE rowid = NEW.rowid; END;
COMMIT;
