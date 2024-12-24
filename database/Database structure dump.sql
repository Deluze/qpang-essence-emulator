-- MySQL dump 10.13  Distrib 8.0.26, for Win64 (x86_64)
--
-- Host: localhost    Database: qfighter
-- ------------------------------------------------------
-- Server version	8.0.26

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `anti_cheat_reports`
--

DROP TABLE IF EXISTS `anti_cheat_reports`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `anti_cheat_reports` (
  `id` int NOT NULL AUTO_INCREMENT,
  `ip_address` varchar(45) NOT NULL,
  `username` varchar(45) DEFAULT NULL,
  `speedhack_enabled` tinyint(1) NOT NULL,
  `debugger_attached` tinyint(1) NOT NULL,
  `illegal_processes` varchar(255) DEFAULT NULL,
  `timestamp` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=230 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `channels`
--

DROP TABLE IF EXISTS `channels`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `channels` (
  `id` bigint unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(191) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL,
  `min_level` tinyint NOT NULL,
  `max_level` tinyint NOT NULL,
  `max_players` int NOT NULL,
  `min_rank` tinyint NOT NULL,
  `ip` varchar(191) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL,
  `created_at` timestamp NULL DEFAULT NULL,
  `updated_at` timestamp NULL DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `crane_items`
--

DROP TABLE IF EXISTS `crane_items`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `crane_items` (
  `id` bigint unsigned NOT NULL AUTO_INCREMENT,
  `item_id` bigint unsigned NOT NULL,
  `item_type` tinyint NOT NULL,
  `use_up` tinyint NOT NULL,
  `period` int NOT NULL,
  `active` tinyint NOT NULL,
  `created_at` timestamp NULL DEFAULT NULL,
  `updated_at` timestamp NULL DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=158 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daily_rewards`
--

DROP TABLE IF EXISTS `daily_rewards`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `daily_rewards` (
  `id` bigint NOT NULL AUTO_INCREMENT,
  `player_id` bigint NOT NULL,
  `streak` int NOT NULL DEFAULT '0',
  `last_activated` bigint NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=171 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `friends`
--

DROP TABLE IF EXISTS `friends`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `friends` (
  `id` bigint unsigned NOT NULL AUTO_INCREMENT,
  `player_from` bigint unsigned NOT NULL,
  `player_to` bigint unsigned NOT NULL,
  `status` tinyint NOT NULL,
  `created_at` timestamp NULL DEFAULT NULL,
  `updated_at` timestamp NULL DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `friends_player_from_foreign` (`player_from`),
  KEY `friends_player_to_foreign` (`player_to`),
  CONSTRAINT `friends_player_from_foreign` FOREIGN KEY (`player_from`) REFERENCES `players` (`id`),
  CONSTRAINT `friends_player_to_foreign` FOREIGN KEY (`player_to`) REFERENCES `players` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1417 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `game_item_spawns`
--

DROP TABLE IF EXISTS `game_item_spawns`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `game_item_spawns` (
  `id` bigint unsigned NOT NULL AUTO_INCREMENT,
  `position_id` bigint unsigned NOT NULL,
  `map_id` bigint unsigned NOT NULL,
  `is_active` tinyint unsigned NOT NULL DEFAULT '1',
  `created_at` timestamp NULL DEFAULT NULL,
  `updated_at` timestamp NULL DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `game_item_spawns_position_id_foreign` (`position_id`),
  KEY `game_item_spawns_map_id_foreign` (`map_id`),
  CONSTRAINT `game_item_spawns_map_id_foreign` FOREIGN KEY (`map_id`) REFERENCES `maps` (`id`),
  CONSTRAINT `game_item_spawns_position_id_foreign` FOREIGN KEY (`position_id`) REFERENCES `positions` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=98 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `game_modes`
--

DROP TABLE IF EXISTS `game_modes`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `game_modes` (
  `id` bigint unsigned NOT NULL AUTO_INCREMENT,
  `mode_id` int unsigned NOT NULL,
  `name` varchar(191) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL,
  `created_at` timestamp NULL DEFAULT NULL,
  `updated_at` timestamp NULL DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `items`
--

DROP TABLE IF EXISTS `items`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `items` (
  `id` bigint unsigned NOT NULL AUTO_INCREMENT,
  `seq_id` int unsigned NOT NULL,
  `item_id` int unsigned NOT NULL,
  `name` varchar(191) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL,
  `type` int unsigned NOT NULL,
  `aid` int unsigned NOT NULL,
  `pay_type` int unsigned NOT NULL,
  `price` int unsigned NOT NULL,
  `use_up` int unsigned NOT NULL,
  `period` int unsigned NOT NULL,
  `level` int unsigned NOT NULL,
  `status` int unsigned NOT NULL,
  `sold_count` int unsigned NOT NULL,
  `stock` int unsigned NOT NULL,
  `order` int unsigned NOT NULL DEFAULT '0',
  `created_at` timestamp NULL DEFAULT NULL,
  `updated_at` timestamp NULL DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=16072 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `levels`
--

DROP TABLE IF EXISTS `levels`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `levels` (
  `id` bigint unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(191) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL,
  `level` int unsigned NOT NULL,
  `experience` int unsigned NOT NULL,
  `don_reward` int unsigned NOT NULL,
  `cash_reward` int unsigned NOT NULL,
  `coin_reward` int unsigned NOT NULL,
  `created_at` timestamp NULL DEFAULT NULL,
  `updated_at` timestamp NULL DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=46 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `map_spawns`
--

DROP TABLE IF EXISTS `map_spawns`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `map_spawns` (
  `id` bigint unsigned NOT NULL AUTO_INCREMENT,
  `map_id` bigint unsigned NOT NULL,
  `game_mode_id` bigint unsigned NOT NULL,
  `position_id` bigint unsigned NOT NULL,
  `team` int unsigned DEFAULT NULL,
  `is_active` tinyint unsigned NOT NULL DEFAULT '1',
  `created_at` timestamp NULL DEFAULT NULL,
  `updated_at` timestamp NULL DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `map_spawns_map_id_foreign` (`map_id`),
  KEY `map_spawns_game_mode_id_foreign` (`game_mode_id`),
  KEY `map_spawns_position_id_foreign` (`position_id`),
  CONSTRAINT `map_spawns_game_mode_id_foreign` FOREIGN KEY (`game_mode_id`) REFERENCES `game_modes` (`id`),
  CONSTRAINT `map_spawns_map_id_foreign` FOREIGN KEY (`map_id`) REFERENCES `maps` (`id`),
  CONSTRAINT `map_spawns_position_id_foreign` FOREIGN KEY (`position_id`) REFERENCES `positions` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=306 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `maps`
--

DROP TABLE IF EXISTS `maps`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `maps` (
  `id` bigint unsigned NOT NULL AUTO_INCREMENT,
  `map_id` int unsigned NOT NULL,
  `name` varchar(191) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL,
  `created_at` timestamp NULL DEFAULT NULL,
  `updated_at` timestamp NULL DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=12 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `memos`
--

DROP TABLE IF EXISTS `memos`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `memos` (
  `id` bigint unsigned NOT NULL AUTO_INCREMENT,
  `sender_id` bigint unsigned NOT NULL,
  `receiver_id` bigint unsigned NOT NULL,
  `message` varchar(191) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL,
  `opened` tinyint NOT NULL,
  `created` int NOT NULL,
  `created_at` timestamp NULL DEFAULT NULL,
  `updated_at` timestamp NULL DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `memos_sender_id_foreign` (`sender_id`),
  KEY `memos_receiver_id_foreign` (`receiver_id`),
  CONSTRAINT `memos_receiver_id_foreign` FOREIGN KEY (`receiver_id`) REFERENCES `players` (`id`),
  CONSTRAINT `memos_sender_id_foreign` FOREIGN KEY (`sender_id`) REFERENCES `players` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `player_achievements`
--

DROP TABLE IF EXISTS `player_achievements`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `player_achievements` (
  `player_id` bigint unsigned NOT NULL,
  `achievement_id` int unsigned NOT NULL,
  KEY `player_achievements_player_id_foreign` (`player_id`),
  CONSTRAINT `player_achievements_player_id_foreign` FOREIGN KEY (`player_id`) REFERENCES `players` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `player_equipment`
--

DROP TABLE IF EXISTS `player_equipment`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `player_equipment` (
  `id` bigint unsigned NOT NULL AUTO_INCREMENT,
  `player_id` bigint unsigned DEFAULT NULL,
  `character_id` int NOT NULL,
  `melee` bigint unsigned DEFAULT NULL,
  `primary` bigint unsigned DEFAULT NULL,
  `secondary` bigint unsigned DEFAULT NULL,
  `throw` bigint unsigned DEFAULT NULL,
  `head` bigint unsigned DEFAULT NULL,
  `face` bigint unsigned DEFAULT NULL,
  `body` bigint unsigned DEFAULT NULL,
  `hands` bigint unsigned DEFAULT NULL,
  `legs` bigint unsigned DEFAULT NULL,
  `shoes` bigint unsigned DEFAULT NULL,
  `back` bigint unsigned DEFAULT NULL,
  `side` bigint unsigned DEFAULT NULL,
  `created_at` timestamp NULL DEFAULT NULL,
  `updated_at` timestamp NULL DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `player_equipment_player_id_foreign` (`player_id`),
  KEY `player_equipment_melee_foreign` (`melee`),
  KEY `player_equipment_primary_foreign` (`primary`),
  KEY `player_equipment_secondary_foreign` (`secondary`),
  KEY `player_equipment_throw_foreign` (`throw`),
  KEY `player_equipment_head_foreign` (`head`),
  KEY `player_equipment_face_foreign` (`face`),
  KEY `player_equipment_body_foreign` (`body`),
  KEY `player_equipment_hands_foreign` (`hands`),
  KEY `player_equipment_legs_foreign` (`legs`),
  KEY `player_equipment_shoes_foreign` (`shoes`),
  KEY `player_equipment_back_foreign` (`back`),
  KEY `player_equipment_side_foreign` (`side`),
  CONSTRAINT `player_equipment_back_foreign` FOREIGN KEY (`back`) REFERENCES `player_items` (`id`),
  CONSTRAINT `player_equipment_body_foreign` FOREIGN KEY (`body`) REFERENCES `player_items` (`id`),
  CONSTRAINT `player_equipment_face_foreign` FOREIGN KEY (`face`) REFERENCES `player_items` (`id`),
  CONSTRAINT `player_equipment_hands_foreign` FOREIGN KEY (`hands`) REFERENCES `player_items` (`id`),
  CONSTRAINT `player_equipment_head_foreign` FOREIGN KEY (`head`) REFERENCES `player_items` (`id`),
  CONSTRAINT `player_equipment_legs_foreign` FOREIGN KEY (`legs`) REFERENCES `player_items` (`id`),
  CONSTRAINT `player_equipment_melee_foreign` FOREIGN KEY (`melee`) REFERENCES `player_items` (`id`),
  CONSTRAINT `player_equipment_player_id_foreign` FOREIGN KEY (`player_id`) REFERENCES `players` (`id`),
  CONSTRAINT `player_equipment_primary_foreign` FOREIGN KEY (`primary`) REFERENCES `player_items` (`id`),
  CONSTRAINT `player_equipment_secondary_foreign` FOREIGN KEY (`secondary`) REFERENCES `player_items` (`id`),
  CONSTRAINT `player_equipment_shoes_foreign` FOREIGN KEY (`shoes`) REFERENCES `player_items` (`id`),
  CONSTRAINT `player_equipment_side_foreign` FOREIGN KEY (`side`) REFERENCES `player_items` (`id`),
  CONSTRAINT `player_equipment_throw_foreign` FOREIGN KEY (`throw`) REFERENCES `player_items` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=3595 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `player_gifts`
--

DROP TABLE IF EXISTS `player_gifts`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `player_gifts` (
  `player_from_id` bigint NOT NULL,
  `player_from_name` varchar(255) NOT NULL,
  `player_to_id` bigint NOT NULL,
  `player_to_name` varchar(255) NOT NULL,
  `card_id` bigint NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `player_items`
--

DROP TABLE IF EXISTS `player_items`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `player_items` (
  `id` bigint unsigned NOT NULL AUTO_INCREMENT,
  `player_id` bigint unsigned NOT NULL,
  `item_id` int NOT NULL,
  `period` int NOT NULL,
  `period_type` int NOT NULL,
  `type` int NOT NULL,
  `active` tinyint(1) NOT NULL DEFAULT '0',
  `opened` tinyint NOT NULL,
  `giftable` tinyint NOT NULL,
  `boosted` tinyint NOT NULL,
  `boost_level` tinyint NOT NULL,
  `time` int NOT NULL,
  `created_at` timestamp NULL DEFAULT NULL,
  `updated_at` timestamp NULL DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=9944861 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `player_skill_cards`
--

DROP TABLE IF EXISTS `player_skill_cards`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `player_skill_cards` (
  `player_id` bigint unsigned NOT NULL,
  `skill_1_card_id` bigint unsigned NOT NULL,
  `skill_2_card_id` bigint unsigned NOT NULL,
  `skill_3_card_id` bigint unsigned NOT NULL,
  PRIMARY KEY (`player_id`),
  CONSTRAINT `player_skills_player_id_fk_id` FOREIGN KEY (`player_id`) REFERENCES `players` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `player_stats`
--

DROP TABLE IF EXISTS `player_stats`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `player_stats` (
  `player_id` bigint unsigned NOT NULL,
  `kills` int unsigned NOT NULL DEFAULT '0',
  `deaths` int unsigned NOT NULL DEFAULT '0',
  `n_won` int unsigned NOT NULL DEFAULT '0',
  `n_lost` int unsigned NOT NULL DEFAULT '0',
  `n_drew` int unsigned NOT NULL DEFAULT '0',
  `m_won` int unsigned NOT NULL DEFAULT '0',
  `m_lost` int unsigned NOT NULL DEFAULT '0',
  `m_drew` int unsigned NOT NULL DEFAULT '0',
  `playtime` int unsigned NOT NULL DEFAULT '0',
  `slacker_points` int unsigned NOT NULL DEFAULT '0',
  `melee_kills` int unsigned NOT NULL DEFAULT '0',
  `gun_kills` int unsigned NOT NULL DEFAULT '0',
  `launcher_kills` int unsigned NOT NULL DEFAULT '0',
  `bomb_kills` int unsigned NOT NULL DEFAULT '0',
  `headshot_kills` int unsigned NOT NULL DEFAULT '0',
  `headshot_deaths` int unsigned NOT NULL DEFAULT '0',
  `team_kills` int unsigned NOT NULL DEFAULT '0',
  `team_deaths` int unsigned NOT NULL DEFAULT '0',
  `event_item_pickups` int unsigned NOT NULL DEFAULT '0',
  `deaths_as_tag` int unsigned NOT NULL DEFAULT '0',
  `deaths_by_tag` int unsigned NOT NULL DEFAULT '0',
  `tag_kills_as_player` int unsigned NOT NULL DEFAULT '0',
  `player_kills_as_tag` int unsigned NOT NULL DEFAULT '0',
  `time_alive_as_tag` int unsigned NOT NULL DEFAULT '0',
  `damage_dealt_to_tag` int unsigned NOT NULL DEFAULT '0',
  `damage_dealt_as_tag` int unsigned NOT NULL DEFAULT '0',
  `public_enemy_games_played` int unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`player_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `players`
--

DROP TABLE IF EXISTS `players`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `players` (
  `id` bigint unsigned NOT NULL AUTO_INCREMENT,
  `user_id` bigint unsigned NOT NULL,
  `name` varchar(191) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL,
  `default_character` int NOT NULL DEFAULT '333',
  `rank` tinyint NOT NULL DEFAULT '1',
  `prestige` tinyint NOT NULL DEFAULT '0',
  `level` tinyint NOT NULL DEFAULT '1',
  `don` int NOT NULL DEFAULT '0',
  `cash` int NOT NULL DEFAULT '0',
  `coins` int NOT NULL DEFAULT '0',
  `experience` int NOT NULL DEFAULT '0',
  `is_muted` tinyint(1) NOT NULL DEFAULT '0',
  `created_at` timestamp NULL DEFAULT NULL,
  `updated_at` timestamp NULL DEFAULT NULL,
  `is_patreon` tinyint(1) NOT NULL DEFAULT '0',
  `daily_bonus_redeemed_at` bigint NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  UNIQUE KEY `name` (`name`),
  KEY `players_user_id_foreign` (`user_id`),
  CONSTRAINT `players_user_id_foreign` FOREIGN KEY (`user_id`) REFERENCES `users` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=600 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `positions`
--

DROP TABLE IF EXISTS `positions`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `positions` (
  `id` bigint unsigned NOT NULL AUTO_INCREMENT,
  `x` double(8,2) NOT NULL,
  `y` double(8,2) NOT NULL,
  `z` double(8,2) NOT NULL,
  `name` varchar(191) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL,
  `created_at` timestamp NULL DEFAULT NULL,
  `updated_at` timestamp NULL DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=562 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `redeem_cards`
--

DROP TABLE IF EXISTS `redeem_cards`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `redeem_cards` (
  `id` int NOT NULL AUTO_INCREMENT,
  `name` varchar(45) NOT NULL,
  `item_id` int unsigned NOT NULL,
  `type` tinyint unsigned NOT NULL,
  `period` int unsigned NOT NULL,
  `period_type` tinyint unsigned NOT NULL,
  `is_giftable` tinyint unsigned NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=30 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='In this table, all cards that can be used for redeem codes get stored.';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `redeem_code_redeem_cards`
--

DROP TABLE IF EXISTS `redeem_code_redeem_cards`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `redeem_code_redeem_cards` (
  `redeem_code_id` int NOT NULL,
  `redeem_card_id` int NOT NULL,
  PRIMARY KEY (`redeem_card_id`,`redeem_code_id`),
  KEY `redeem_code_redeem_cards_redeem_code_fk_id_idx` (`redeem_code_id`),
  CONSTRAINT `redeem_code_redeem_cards_redeem_card_fk_id` FOREIGN KEY (`redeem_card_id`) REFERENCES `redeem_cards` (`id`),
  CONSTRAINT `redeem_code_redeem_cards_redeem_code_fk_id` FOREIGN KEY (`redeem_code_id`) REFERENCES `redeem_codes` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='In this table, the relation between a redeem code and a card gets stored.';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `redeem_code_uses`
--

DROP TABLE IF EXISTS `redeem_code_uses`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `redeem_code_uses` (
  `redeem_code_id` int NOT NULL,
  `player_id` bigint unsigned NOT NULL,
  `use_count` int unsigned NOT NULL,
  PRIMARY KEY (`redeem_code_id`,`player_id`),
  KEY `redeem_code_id_fk_idx` (`redeem_code_id`) /*!80000 INVISIBLE */,
  KEY `redeem_code_uses_player_fk_id_idx` (`player_id`),
  CONSTRAINT `redeem_code_uses_player_fk_id` FOREIGN KEY (`player_id`) REFERENCES `players` (`id`),
  CONSTRAINT `redeem_code_uses_redeem_code_fk_id` FOREIGN KEY (`redeem_code_id`) REFERENCES `redeem_codes` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='In this table, the amount of times a certain player has used a redeem code gets stored.';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `redeem_codes`
--

DROP TABLE IF EXISTS `redeem_codes`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `redeem_codes` (
  `id` int NOT NULL AUTO_INCREMENT,
  `player_id` bigint unsigned NOT NULL DEFAULT '0',
  `redeem_code` char(14) NOT NULL,
  `total_uses` int unsigned NOT NULL DEFAULT '0',
  `use_count` int unsigned NOT NULL DEFAULT '0',
  `cash` int unsigned NOT NULL DEFAULT '0',
  `don` int unsigned NOT NULL DEFAULT '0',
  `active` tinyint unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  UNIQUE KEY `redeem_code_UNIQUE` (`redeem_code`)
) ENGINE=InnoDB AUTO_INCREMENT=26 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='In this table, all of the redeem codes get stored.';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `user_bans`
--

DROP TABLE IF EXISTS `user_bans`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `user_bans` (
  `id` bigint unsigned NOT NULL AUTO_INCREMENT,
  `user_id` bigint unsigned NOT NULL,
  `timestamp_ban` int unsigned NOT NULL,
  `timestamp_unban` int unsigned NOT NULL,
  `created_at` timestamp NULL DEFAULT NULL,
  `updated_at` timestamp NULL DEFAULT NULL,
  `banned_by_user_id` bigint unsigned NOT NULL,
  PRIMARY KEY (`id`),
  KEY `user_bans_user_id_foreign` (`user_id`),
  CONSTRAINT `user_bans_user_id_foreign` FOREIGN KEY (`user_id`) REFERENCES `users` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=19 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `user_ips`
--

DROP TABLE IF EXISTS `user_ips`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `user_ips` (
  `user_id` bigint unsigned NOT NULL,
  `ip` varchar(50) NOT NULL,
  KEY `FK_user_ips_users` (`user_id`),
  CONSTRAINT `FK_user_ips_users` FOREIGN KEY (`user_id`) REFERENCES `users` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `users`
--

DROP TABLE IF EXISTS `users`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `users` (
  `id` bigint unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(191) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL,
  `email` varchar(191) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL,
  `password` varchar(191) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL,
  `session_uuid` varchar(191) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL DEFAULT '',
  `registration_ip` varchar(50) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `whitelisted` tinyint(1) NOT NULL DEFAULT '0',
  `created_at` timestamp NULL DEFAULT NULL,
  `updated_at` timestamp NULL DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=600 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `weapons`
--

DROP TABLE IF EXISTS `weapons`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `weapons` (
  `id` bigint unsigned NOT NULL AUTO_INCREMENT,
  `code_name` varchar(191) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL,
  `item_id` int unsigned NOT NULL,
  `damage` int unsigned NOT NULL,
  `clip_size` int NOT NULL DEFAULT '0',
  `clip_amount` int NOT NULL DEFAULT '0',
  `weight` tinyint unsigned NOT NULL DEFAULT '0',
  `effect_id` tinyint unsigned DEFAULT NULL,
  `chance` tinyint unsigned DEFAULT NULL,
  `duration` int unsigned DEFAULT NULL,
  `created_at` timestamp NULL DEFAULT NULL,
  `updated_at` timestamp NULL DEFAULT NULL,
  `type` int DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=172 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2021-12-18 14:11:16
