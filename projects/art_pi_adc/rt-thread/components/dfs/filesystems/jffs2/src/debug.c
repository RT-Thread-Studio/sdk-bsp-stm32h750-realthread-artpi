/*
 * JFFS2 -- Journalling Flash File System, Version 2.
 *
 * Copyright (C) 2001-2003 Red Hat, Inc.
 *
 * Created by David Woodhouse <dwmw2@infradead.org>
 *
 * For licensing information, see the file 'LICENCE' in this directory.
 *
 * $Id: debug.c,v 1.1 2005/07/30 15:30:42 asl Exp $
 *
 */
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/pagemap.h>
#include <linux/crc32.h>
#include <linux/jffs2.h>
#include <linux/slab.h>
#include "nodelist.h"
#include "debug.h"

#ifdef JFFS2_DBG_PARANOIA_CHECKS
/*
 * Check the fragtree.
 */
void
__jffs2_dbg_fragtree_paranoia_check(struct jffs2_inode_info *f)
{
	down(&f->sem);
	__jffs2_dbg_fragtree_paranoia_check_nolock(f);
	up(&f->sem);
}
	
void
__jffs2_dbg_fragtree_paranoia_check_nolock(struct jffs2_inode_info *f)
{
	struct jffs2_node_frag *frag;
	int bitched = 0;

	for (frag = frag_first(&f->fragtree); frag; frag = frag_next(frag)) {
		struct jffs2_full_dnode *fn = frag->node;

		if (!fn || !fn->raw)
			continue;

		if (ref_flags(fn->raw) == REF_PRISTINE) {
			if (fn->frags > 1) {
				JFFS2_ERROR("REF_PRISTINE node at 0x%08x had %d frags. Tell dwmw2.\n",
						ref_offset(fn->raw), fn->frags);
				bitched = 1;
			}

			/* A hole node which isn't multi-page should be garbage-collected
			   and merged anyway, so we just check for the frag size here,
			   rather than mucking around with actually reading the node
			   and checking the compression type, which is the real way
			   to tell a hole node. */
			if (frag->ofs & (PAGE_CACHE_SIZE-1) && frag_prev(frag)
					&& frag_prev(frag)->size < PAGE_CACHE_SIZE && frag_prev(frag)->node) {
				JFFS2_ERROR("REF_PRISTINE node at 0x%08x had a previous non-hole frag "
						"in the same page. Tell dwmw2.\n", ref_offset(fn->raw));
				bitched = 1;
			}

			if ((frag->ofs+frag->size) & (PAGE_CACHE_SIZE-1) && frag_next(frag)
					&& frag_next(frag)->size < PAGE_CACHE_SIZE && frag_next(frag)->node) {
				JFFS2_ERROR("REF_PRISTINE node at 0x%08x (%08x-%08x) had a following "
						"non-hole frag in the same page. Tell dwmw2.\n",
					       ref_offset(fn->raw), frag->ofs, frag->ofs+frag->size);
				bitched = 1;
			}
		}
	}

	if (bitched) {
		JFFS2_ERROR("fragtree is corrupted.\n");
		__jffs2_dbg_dump_fragtree_nolock(f);
		BUG();
	}
}

/*
 * Check if the flash contains all 0xFF before we start writing.
 */
void
__jffs2_dbg_prewrite_paranoia_check(struct jffs2_sb_info *c,
				    uint32_t ofs, int len)
{
	size_t retlen;
	int ret, i;
	unsigned char *buf;

	buf = kmalloc(len, GFP_KERNEL);
	if (!buf)
		return;

	ret = jffs2_flash_read(c, ofs, len, &retlen, buf);
	if (ret || (retlen != len)) {
		JFFS2_WARNING("read %d bytes failed or short. ret %d, retlen %zd.\n",
				len, ret, retlen);
		kfree(buf);
		return;
	}

	ret = 0;
	for (i = 0; i < len; i++)
		if (buf[i] != 0xff)
			ret = 1;

	if (ret) {
		JFFS2_ERROR("argh, about to write node to %#08x on flash, but there are data "
			"already there. The first corrupted byte is at %#08x offset.\n", ofs, ofs + i);
		__jffs2_dbg_dump_buffer(buf, len, ofs);
		kfree(buf);
		BUG();
	}

	kfree(buf);
}

/*
 * Check the space accounting and node_ref list correctness for the JFFS2 erasable block 'jeb'.
 */
void
__jffs2_dbg_acct_paranoia_check(struct jffs2_sb_info *c,
				struct jffs2_eraseblock *jeb)
{
	spin_lock(&c->erase_completion_lock);
	__jffs2_dbg_acct_paranoia_check_nolock(c, jeb);
	spin_unlock(&c->erase_completion_lock);
}
	
void
__jffs2_dbg_acct_paranoia_check_nolock(struct jffs2_sb_info *c,
				       struct jffs2_eraseblock *jeb)
{
	uint32_t my_used_size = 0;
	uint32_t my_unchecked_size = 0;
	uint32_t my_dirty_size = 0;
	struct jffs2_raw_node_ref *ref2 = jeb->first_node;

	while (ref2) {
		uint32_t totlen = ref_totlen(c, jeb, ref2);

		if (ref2->flash_offset < jeb->offset ||
				ref2->flash_offset > jeb->offset + c->sector_size) {
			JFFS2_ERROR("node_ref %#08x shouldn't be in block at %#08x.\n",
				ref_offset(ref2), jeb->offset);
			goto error;

		}
		if (ref_flags(ref2) == REF_UNCHECKED)
			my_unchecked_size += totlen;
		else if (!ref_obsolete(ref2))
			my_used_size += totlen;
		else
			my_dirty_size += totlen;

		if ((!ref2->next_phys) != (ref2 == jeb->last_node)) {
			JFFS2_ERROR("node_ref for node at %#08x (mem %p) has next_phys at %#08x (mem %p), "
				"last_node is at %#08x (mem %p).\n",
				ref_offset(ref2), ref2, ref_offset(ref2->next_phys), ref2->next_phys,
				ref_offset(jeb->last_node), jeb->last_node);
			goto error;
		}
		ref2 = ref2->next_phys;
	}

	if (my_used_size != jeb->used_size) {
		JFFS2_ERROR("Calculated used size %#08x != stored used size %#08x.\n",
			my_used_size, jeb->used_size);
		goto error;
	}

	if (my_unchecked_size != jeb->unchecked_size) {
		JFFS2_ERROR("Calculated unchecked size %#08x != stored unchecked size %#08x.\n",
			my_unchecked_size, jeb->unchecked_size);
		goto error;
	}

#if 0
	/* This should work when we implement ref->__totlen elemination */
	if (my_dirty_size != jeb->dirty_size + jeb->wasted_size) {
		JFFS2_ERROR("Calculated dirty+wasted size %#08x != stored dirty + wasted size %#08x\n",
			my_dirty_size, jeb->dirty_size + jeb->wasted_size);
		goto error;
	}

	if (jeb->free_size == 0
		&& my_used_size + my_unchecked_size + my_dirty_size != c->sector_size) {
		JFFS2_ERROR("The sum of all nodes in block (%#x) != size of block (%#x)\n",
			my_used_size + my_unchecked_size + my_dirty_size,
			c->sector_size);
		goto error;
	}
#endif

	return;

error:
	__jffs2_dbg_dump_node_refs_nolock(c, jeb);
	__jffs2_dbg_dump_jeb_nolock(jeb);
	__jffs2_dbg_dump_block_lists_nolock(c);
	BUG();
	
}
#endif /* JFFS2_DBG_PARANOIA_CHECKS */

#if defined(JFFS2_DBG_DUMPS) || defined(JFFS2_DBG_PARANOIA_CHECKS)
/*
 * Dump the node_refs of the 'jeb' JFFS2 eraseblock.
 */
void
__jffs2_dbg_dump_node_refs(struct jffs2_sb_info *c,
			   struct jffs2_eraseblock *jeb)
{
	spin_lock(&c->erase_completion_lock);
	__jffs2_dbg_dump_node_refs_nolock(c, jeb);
	spin_unlock(&c->erase_completion_lock);
}

void
__jffs2_dbg_dump_node_refs_nolock(struct jffs2_sb_info *c,
				  struct jffs2_eraseblock *jeb)
{
	struct jffs2_raw_node_ref *ref;
	int i = 0;

	JFFS2_DEBUG("Dump node_refs of the eraseblock %#08x\n", jeb->offset);
	if (!jeb->first_node) {
		JFFS2_DEBUG("no nodes in the eraseblock %#08x\n", jeb->offset);
		return;
	}

	printk(JFFS2_DBG_LVL);
	for (ref = jeb->first_node; ; ref = ref->next_phys) {
		printk("%#08x(%#x)", ref_offset(ref), ref->__totlen);
		if (ref->next_phys)
			printk("->");
		else
			break;
		if (++i == 4) {
			i = 0;
			printk("\n" JFFS2_DBG_LVL);
		}
	}
	printk("\n");
}

/*
 * Dump an eraseblock's space accounting.
 */
void
__jffs2_dbg_dump_jeb(struct jffs2_sb_info *c, struct jffs2_eraseblock *jeb)
{
	spin_lock(&c->erase_completion_lock);
	__jffs2_dbg_dump_jeb_nolock(jeb);
	spin_unlock(&c->erase_completion_lock);
}

void
__jffs2_dbg_dump_jeb_nolock(struct jffs2_eraseblock *jeb)
{
	if (!jeb)
		return;

	JFFS2_DEBUG("dump space accounting for the eraseblock at %#08x:\n",
			jeb->offset);

	printk(JFFS2_DBG_LVL "used_size: %#08x\n",	jeb->used_size);
	printk(JFFS2_DBG_LVL "dirty_size: %#08x\n",	jeb->dirty_size);
	printk(JFFS2_DBG_LVL "wasted_size: %#08x\n",	jeb->wasted_size);
	printk(JFFS2_DBG_LVL "unchecked_size: %#08x\n",	jeb->unchecked_size);
	printk(JFFS2_DBG_LVL "free_size: %#08x\n",	jeb->free_size);
}

void
__jffs2_dbg_dump_block_lists(struct jffs2_sb_info *c)
{
	spin_lock(&c->erase_completion_lock);
	__jffs2_dbg_dump_block_lists_nolock(c);
	spin_unlock(&c->erase_completion_lock);
}

void
__jffs2_dbg_dump_block_lists_nolock(struct jffs2_sb_info *c)
{
	JFFS2_DEBUG("dump JFFS2 blocks lists:\n");
	
	printk(JFFS2_DBG_LVL "flash_size: %#08x\n",	c->flash_size);
	printk(JFFS2_DBG_LVL "used_size: %#08x\n",	c->used_size);
	printk(JFFS2_DBG_LVL "dirty_size: %#08x\n",	c->dirty_size);
	printk(JFFS2_DBG_LVL "wasted_size: %#08x\n",	c->wasted_size);
	printk(JFFS2_DBG_LVL "unchecked_size: %#08x\n",	c->unchecked_size);
	printk(JFFS2_DBG_LVL "free_size: %#08x\n",	c->free_size);
	printk(JFFS2_DBG_LVL "erasing_size: %#08x\n",	c->erasing_size);
	printk(JFFS2_DBG_LVL "bad_size: %#08x\n",	c->bad_size);
	printk(JFFS2_DBG_LVL "sector_size: %#08x\n",	c->sector_size);
	printk(JFFS2_DBG_LVL "jffs2_reserved_blocks size: %#08x\n",
				c->sector_size * c->resv_blocks_write);

	if (c->nextblock)
		printk(JFFS2_DBG_LVL "nextblock: %#08x (used %#08x, dirty %#08x, wasted %#08x, "
			"unchecked %#08x, free %#08x)\n",
			c->nextblock->offset, c->nextblock->used_size,
			c->nextblock->dirty_size, c->nextblock->wasted_size,
			c->nextblock->unchecked_size, c->nextblock->free_size);
	else
		printk(JFFS2_DBG_LVL "nextblock: NULL\n");

	if (c->gcblock)
		printk(JFFS2_DBG_LVL "gcblock: %#08x (used %#08x, dirty %#08x, wasted %#08x, "
			"unchecked %#08x, free %#08x)\n",
			c->gcblock->offset, c->gcblock->used_size, c->gcblock->dirty_size,
			c->gcblock->wasted_size, c->gcblock->unchecked_size, c->gcblock->free_size);
	else
		printk(JFFS2_DBG_LVL "gcblock: NULL\n");

	if (list_empty(&c->clean_list)) {
		printk(JFFS2_DBG_LVL "clean_list: empty\n");
	} else {
		struct list_head *this;
		int numblocks = 0;
		uint32_t dirty = 0;

		list_for_each(this, &c->clean_list) {
			struct jffs2_eraseblock *jeb = list_entry(this, struct jffs2_eraseblock, list);
			numblocks ++;
			dirty += jeb->wasted_size;
			if (!(jeb->used_size == 0 && jeb->dirty_size == 0 && jeb->wasted_size == 0)) {
				printk(JFFS2_DBG_LVL "clean_list: %#08x (used %#08x, dirty %#08x, wasted %#08x, "
					"unchecked %#08x, free %#08x)\n",
					jeb->offset, jeb->used_size, jeb->dirty_size, jeb->wasted_size,
					jeb->unchecked_size, jeb->free_size);
			}
		}

		printk (JFFS2_DBG_LVL "Contains %d blocks with total wasted size %u, average wasted size: %u\n",
			numblocks, dirty, dirty / numblocks);
	}

	if (list_empty(&c->very_dirty_list)) {
		printk(JFFS2_DBG_LVL "very_dirty_list: empty\n");
	} else {
		struct list_head *this;
		int numblocks = 0;
		uint32_t dirty = 0;

		list_for_each(this, &c->very_dirty_list) {
			struct jffs2_eraseblock *jeb = list_entry(this, struct jffs2_eraseblock, list);

			numblocks ++;
			dirty += jeb->dirty_size;
			if (!(jeb->used_size == 0 && jeb->dirty_size == 0 && jeb->wasted_size == 0)) {
				printk(JFFS2_DBG_LVL "very_dirty_list: %#08x (used %#08x, dirty %#08x, wasted %#08x, "
					"unchecked %#08x, free %#08x)\n",
					jeb->offset, jeb->used_size, jeb->dirty_size, jeb->wasted_size,
					jeb->unchecked_size, jeb->free_size);
			}
		}

		printk (JFFS2_DBG_LVL "Contains %d blocks with total dirty size %u, average dirty size: %u\n",
			numblocks, dirty, dirty / numblocks);
	}

	if (list_empty(&c->dirty_list)) {
		printk(JFFS2_DBG_LVL "dirty_list: empty\n");
	} else {
		struct list_head *this;
		int numblocks = 0;
		uint32_t dirty = 0;

		list_for_each(this, &c->dirty_list) {
			struct jffs2_eraseblock *jeb = list_entry(this, struct jffs2_eraseblock, list);

			numblocks ++;
			dirty += jeb->dirty_size;
			if (!(jeb->used_size == 0 && jeb->dirty_size == 0 && jeb->wasted_size == 0)) {
				printk(JFFS2_DBG_LVL "dirty_list: %#08x (used %#08x, dirty %#08x, wasted %#08x, "
					"unchecked %#08x, free %#08x)\n",
					jeb->offset, jeb->used_size, jeb->dirty_size, jeb->wasted_size,
					jeb->unchecked_size, jeb->free_size);
			}
		}

		printk (JFFS2_DBG_LVL "contains %d blocks with total dirty size %u, average dirty size: %u\n",
			numblocks, dirty, dirty / numblocks);
	}

	if (list_empty(&c->erasable_list)) {
		printk(JFFS2_DBG_LVL "erasable_list: empty\n");
	} else {
		struct list_head *this;

		list_for_each(this, &c->erasable_list) {
			struct jffs2_eraseblock *jeb = list_entry(this, struct jffs2_eraseblock, list);

			if (!(jeb->used_size == 0 && jeb->dirty_size == 0 && jeb->wasted_size == 0)) {
				printk(JFFS2_DBG_LVL "erasable_list: %#08x (used %#08x, dirty %#08x, wasted %#08x, "
					"unchecked %#08x, free %#08x)\n",
					jeb->offset, jeb->used_size, jeb->dirty_size, jeb->wasted_size,
					jeb->unchecked_size, jeb->free_size);
			}
		}
	}

	if (list_empty(&c->erasing_list)) {
		printk(JFFS2_DBG_LVL "erasing_list: empty\n");
	} else {
		struct list_head *this;

		list_for_each(this, &c->erasing_list) {
			struct jffs2_eraseblock *jeb = list_entry(this, struct jffs2_eraseblock, list);

			if (!(jeb->used_size == 0 && jeb->dirty_size == 0 && jeb->wasted_size == 0)) {
				printk(JFFS2_DBG_LVL "erasing_list: %#08x (used %#08x, dirty %#08x, wasted %#08x, "
					"unchecked %#08x, free %#08x)\n",
					jeb->offset, jeb->used_size, jeb->dirty_size, jeb->wasted_size,
					jeb->unchecked_size, jeb->free_size);
			}
		}
	}

	if (list_empty(&c->erase_pending_list)) {
		printk(JFFS2_DBG_LVL "erase_pending_list: empty\n");
	} else {
		struct list_head *this;

		list_for_each(this, &c->erase_pending_list) {
			struct jffs2_eraseblock *jeb = list_entry(this, struct jffs2_eraseblock, list);

			if (!(jeb->used_size == 0 && jeb->dirty_size == 0 && jeb->wasted_size == 0)) {
				printk(JFFS2_DBG_LVL "erase_pending_list: %#08x (used %#08x, dirty %#08x, wasted %#08x, "
					"unchecked %#08x, free %#08x)\n",
					jeb->offset, jeb->used_size, jeb->dirty_size, jeb->wasted_size,
					jeb->unchecked_size, jeb->free_size);
			}
		}
	}

	if (list_empty(&c->erasable_pending_wbuf_list)) {
		printk(JFFS2_DBG_LVL "erasable_pending_wbuf_list: empty\n");
	} else {
		struct list_head *this;

		list_for_each(this, &c->erasable_pending_wbuf_list) {
			struct jffs2_eraseblock *jeb = list_entry(this, struct jffs2_eraseblock, list);

			if (!(jeb->used_size == 0 && jeb->dirty_size == 0 && jeb->wasted_size == 0)) {
				printk(JFFS2_DBG_LVL "erasable_pending_wbuf_list: %#08x (used %#08x, dirty %#08x, "
					"wasted %#08x, unchecked %#08x, free %#08x)\n",
					jeb->offset, jeb->used_size, jeb->dirty_size, jeb->wasted_size,
					jeb->unchecked_size, jeb->free_size);
			}
		}
	}

	if (list_empty(&c->free_list)) {
		printk(JFFS2_DBG_LVL "free_list: empty\n");
	} else {
		struct list_head *this;

		list_for_each(this, &c->free_list) {
			struct jffs2_eraseblock *jeb = list_entry(this, struct jffs2_eraseblock, list);

			if (!(jeb->used_size == 0 && jeb->dirty_size == 0 && jeb->wasted_size == 0)) {
				printk(JFFS2_DBG_LVL "free_list: %#08x (used %#08x, dirty %#08x, wasted %#08x, "
					"unchecked %#08x, free %#08x)\n",
					jeb->offset, jeb->used_size, jeb->dirty_size, jeb->wasted_size,
					jeb->unchecked_size, jeb->free_size);
			}
		}
	}

	if (list_empty(&c->bad_list)) {
		printk(JFFS2_DBG_LVL "bad_list: empty\n");
	} else {
		struct list_head *this;

		list_for_each(this, &c->bad_list) {
			struct jffs2_eraseblock *jeb = list_entry(this, struct jffs2_eraseblock, list);

			if (!(jeb->used_size == 0 && jeb->dirty_size == 0 && jeb->wasted_size == 0)) {
				printk(JFFS2_DBG_LVL "bad_list: %#08x (used %#08x, dirty %#08x, wasted %#08x, "
					"unchecked %#08x, free %#08x)\n",
					jeb->offset, jeb->used_size, jeb->dirty_size, jeb->wasted_size,
					jeb->unchecked_size, jeb->free_size);
			}
		}
	}

	if (list_empty(&c->bad_used_list)) {
		printk(JFFS2_DBG_LVL "bad_used_list: empty\n");
	} else {
		struct list_head *this;

		list_for_each(this, &c->bad_used_list) {
			struct jffs2_eraseblock *jeb = list_entry(this, struct jffs2_eraseblock, list);

			if (!(jeb->used_size == 0 && jeb->dirty_size == 0 && jeb->wasted_size == 0)) {
				printk(JFFS2_DBG_LVL "bad_used_list: %#08x (used %#08x, dirty %#08x, wasted %#08x, "
					"unchecked %#08x, free %#08x)\n",
					jeb->offset, jeb->used_size, jeb->dirty_size, jeb->wasted_size,
					jeb->unchecked_size, jeb->free_size);
			}
		}
	}
}

void
__jffs2_dbg_dump_fragtree(struct jffs2_inode_info *f)
{
	down(&f->sem);
	jffs2_dbg_dump_fragtree_nolock(f);
	up(&f->sem);
}

void
__jffs2_dbg_dump_fragtree_nolock(struct jffs2_inode_info *f)
{
	struct jffs2_node_frag *this = frag_first(&f->fragtree);
	uint32_t lastofs = 0;
	int buggy = 0;

	JFFS2_DEBUG("dump fragtree of ino #%u\n", f->inocache->ino);
	while(this) {
		if (this->node)
			printk(JFFS2_DBG_LVL "frag %#04x-%#04x: %#08x(%d) on flash (*%p), left (%p), "
				"right (%p), parent (%p)\n",
				this->ofs, this->ofs+this->size, ref_offset(this->node->raw),
				ref_flags(this->node->raw), this, frag_left(this), frag_right(this),
				frag_parent(this));
		else
			printk(JFFS2_DBG_LVL "frag %#04x-%#04x: hole (*%p). left (%p), right (%p), parent (%p)\n",
				this->ofs, this->ofs+this->size, this, frag_left(this),
				frag_right(this), frag_parent(this));
		if (this->ofs != lastofs)
			buggy = 1;
		lastofs = this->ofs + this->size;
		this = frag_next(this);
	}

	if (f->metadata)
		printk(JFFS2_DBG_LVL "metadata at 0x%08x\n", ref_offset(f->metadata->raw));

	if (buggy) {
		JFFS2_ERROR("frag tree got a hole in it.\n");
		BUG();
	}
}

#define JFFS2_BUFDUMP_BYTES_PER_LINE	32
void
__jffs2_dbg_dump_buffer(unsigned char *buf, int len, uint32_t offs)
{
	int skip;
	int i;
	
	JFFS2_DEBUG("dump from offset %#08x to offset %#08x (%x bytes).\n",
		offs, offs + len, len);
	i = skip = offs % JFFS2_BUFDUMP_BYTES_PER_LINE;
	offs = offs & ~(JFFS2_BUFDUMP_BYTES_PER_LINE - 1);
	
	if (skip != 0)
		printk(JFFS2_DBG_LVL "%#08x: ", offs);
	
	while (skip--)
		printk("   ");

	while (i < len) {
		if ((i % JFFS2_BUFDUMP_BYTES_PER_LINE) == 0 && i != len -1) {
			if (i != 0)
				printk("\n");
			offs += JFFS2_BUFDUMP_BYTES_PER_LINE;
			printk(JFFS2_DBG_LVL "%0#8x: ", offs);
		}

		printk("%02x ", buf[i]);
		
		i += 1;
	}

	printk("\n");
}

/*
 * Dump a JFFS2 node.
 */
void
__jffs2_dbg_dump_node(struct jffs2_sb_info *c, uint32_t ofs)
{
	union jffs2_node_union node;
	int len = sizeof(union jffs2_node_union);
	size_t retlen;
	uint32_t crc;
	int ret;
	
	JFFS2_DEBUG("dump node at offset %#08x.\n", ofs);

	ret = jffs2_flash_read(c, ofs, len, &retlen, (unsigned char *)&node);
	if (ret || (retlen != len)) {
		JFFS2_ERROR("read %d bytes failed or short. ret %d, retlen %zd.\n",
			len, ret, retlen);
		return;
	}

	printk(JFFS2_DBG_LVL "magic:\t%#04x\n",
		je16_to_cpu(node.u.magic));
	printk(JFFS2_DBG_LVL "nodetype:\t%#04x\n",
		je16_to_cpu(node.u.nodetype));
	printk(JFFS2_DBG_LVL "totlen:\t%#08x\n",
		je32_to_cpu(node.u.totlen));
	printk(JFFS2_DBG_LVL "hdr_crc:\t%#08x\n",
		je32_to_cpu(node.u.hdr_crc));
	
	crc = crc32(0, &node.u, sizeof(node.u) - 4);
	if (crc != je32_to_cpu(node.u.hdr_crc)) {
		JFFS2_ERROR("wrong common header CRC.\n");
		return;
	}
	
	if (je16_to_cpu(node.u.magic) != JFFS2_MAGIC_BITMASK &&
		je16_to_cpu(node.u.magic) != JFFS2_OLD_MAGIC_BITMASK)
	{
		JFFS2_ERROR("wrong node magic: %#04x instead of %#04x.\n",
			je16_to_cpu(node.u.magic), JFFS2_MAGIC_BITMASK);
		return;
	}

	switch(je16_to_cpu(node.u.nodetype)) {

	case JFFS2_NODETYPE_INODE:

		printk(JFFS2_DBG_LVL "the node is inode node\n");
		printk(JFFS2_DBG_LVL "ino:\t%#08x\n",
				je32_to_cpu(node.i.ino));
		printk(JFFS2_DBG_LVL "version:\t%#08x\n",
				je32_to_cpu(node.i.version));
		printk(JFFS2_DBG_LVL "mode:\t%#08x\n",
				node.i.mode.m);
		printk(JFFS2_DBG_LVL "uid:\t%#04x\n",
				je16_to_cpu(node.i.uid));
		printk(JFFS2_DBG_LVL "gid:\t%#04x\n",
				je16_to_cpu(node.i.gid));
		printk(JFFS2_DBG_LVL "isize:\t%#08x\n",
				je32_to_cpu(node.i.isize));
		printk(JFFS2_DBG_LVL "atime:\t%#08x\n",
				je32_to_cpu(node.i.atime));
		printk(JFFS2_DBG_LVL "mtime:\t%#08x\n",
				je32_to_cpu(node.i.mtime));
		printk(JFFS2_DBG_LVL "ctime:\t%#08x\n",
				je32_to_cpu(node.i.ctime));
		printk(JFFS2_DBG_LVL "offset:\t%#08x\n",
				je32_to_cpu(node.i.offset));
		printk(JFFS2_DBG_LVL "csize:\t%#08x\n",
				je32_to_cpu(node.i.csize));
		printk(JFFS2_DBG_LVL "dsize:\t%#08x\n",
				je32_to_cpu(node.i.dsize));
		printk(JFFS2_DBG_LVL "compr:\t%#02x\n",
				node.i.compr);
		printk(JFFS2_DBG_LVL "usercompr:\t%#02x\n",
				node.i.usercompr);
		printk(JFFS2_DBG_LVL "flags:\t%#04x\n",
				je16_to_cpu(node.i.flags));
		printk(JFFS2_DBG_LVL "data_crc:\t%#08x\n",
				je32_to_cpu(node.i.data_crc));
		printk(JFFS2_DBG_LVL "node_crc:\t%#08x\n",
				je32_to_cpu(node.i.node_crc));
		crc = crc32(0, &node.i, sizeof(node.i) - 8); 
		if (crc != je32_to_cpu(node.i.node_crc)) {
			JFFS2_ERROR("wrong node header CRC.\n");
			return;
		}
		break;

	case JFFS2_NODETYPE_DIRENT:

		printk(JFFS2_DBG_LVL "the node is dirent node\n");
		printk(JFFS2_DBG_LVL "pino:\t%#08x\n",
				je32_to_cpu(node.d.pino));
		printk(JFFS2_DBG_LVL "version:\t%#08x\n",
				je32_to_cpu(node.d.version));
		printk(JFFS2_DBG_LVL "ino:\t%#08x\n",
				je32_to_cpu(node.d.ino));
		printk(JFFS2_DBG_LVL "mctime:\t%#08x\n",
				je32_to_cpu(node.d.mctime));
		printk(JFFS2_DBG_LVL "nsize:\t%#02x\n",
				node.d.nsize);
		printk(JFFS2_DBG_LVL "type:\t%#02x\n",
				node.d.type);
		printk(JFFS2_DBG_LVL "node_crc:\t%#08x\n",
				je32_to_cpu(node.d.node_crc));
		printk(JFFS2_DBG_LVL "name_crc:\t%#08x\n",
				je32_to_cpu(node.d.name_crc));
		
		node.d.name[node.d.nsize] = '\0';
		printk(JFFS2_DBG_LVL "name:\t\"%s\"\n", node.d.name);

		crc = crc32(0, &node.d, sizeof(node.d) - 8); 
		if (crc != je32_to_cpu(node.d.node_crc)) {
			JFFS2_ERROR("wrong node header CRC.\n");
			return;
		}
		break;

	default:
		printk(JFFS2_DBG_LVL "node type is unknown\n");
		break;
	}
}
#endif /* JFFS2_DBG_DUMPS || JFFS2_DBG_PARANOIA_CHECKS */
