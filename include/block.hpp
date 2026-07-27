#pragma once

enum class BlockType {
	Empty = 0,
	SolidProto
};

struct Block {
	BlockType type = BlockType::Empty;
	int hp = 0;
	bool unbreakable = false;

	bool isSolid() const {
		return type == BlockType::SolidProto;
	}

	bool canBeDamaged() const {
		return isSolid() && !unbreakable;
	}

	bool applyDamage(int damageAmount) {
		if (!canBeDamaged() || damageAmount <= 0) {
			return false;
		}

		hp -= damageAmount;
		if (hp > 0) {
			return false;
		}

		type = BlockType::Empty;
		hp = 0;
		unbreakable = false;
		return true;
	}
};
