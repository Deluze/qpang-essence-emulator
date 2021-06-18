ALTER TABLE `qfighter`.`user_bans` 
ADD COLUMN `banned_by_user_id` BIGINT(20) UNSIGNED NOT NULL AFTER `updated_at`;

CREATE TABLE `qfighter`.`player_gifts` (
  `player_from_id` BIGINT(20) NOT NULL,
  `player_from_name` VARCHAR(255) NOT NULL,
  `player_to_id` BIGINT(20) NOT NULL,
  `player_to_name` VARCHAR(255) NOT NULL,
  `card_id` BIGINT(20) NOT NULL
)