room
room_key pkey auto_increment
room_status smallint not null
room_name varchar(255) not null
room_password password(encrypted) default null
room_capacity smallint not null
room_level smallint not null
created_at datetime not null
last_changed datetime not null

level_type
level_type_id smallint key -> room_level
level_type_desc varchar255 not null {'easy', 'medium', 'hard'}

status_type
status_type_id smallint key -> room_status
status_type_desc varchar255 not null {'created', 'in game', 'closed'}

room_player
room_id smallint key -> room_key
player_id smallint key -> player_key
room_player_type smallint not null

room_player_type
room_player_type_id smallint key -> room_player_type
room_player_type_desc varchar255 not null {'owner', 'player', 'spectator'}

player
player_key pkey auto_increment
player_name varchar(255) not null
