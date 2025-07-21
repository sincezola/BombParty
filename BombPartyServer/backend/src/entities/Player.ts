export interface PlayerProps {
  player_key: number;
  player_name: string;
}

export class Player {
  constructor(private playerProps: PlayerProps) {}

  get player_key() {
    return this.playerProps.player_key;
  }

  get player_name() {
    return this.playerProps.player_name;
  }
}
