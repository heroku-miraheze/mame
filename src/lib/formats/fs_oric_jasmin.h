// license:BSD-3-Clause
// copyright-holders:Olivier Galibert

// Management of Oric Jasmin floppy images

#ifndef MAME_FORMATS_FS_ORIC_JASMIN_H
#define MAME_FORMATS_FS_ORIC_JASMIN_H

#pragma once

#include "fsmgr.h"

namespace fs {

class oric_jasmin_image : public manager_t {
public:
	class impl : public filesystem_t {
	public:
		class root_dir : public idir_t {
		public:
			root_dir(impl &i) : m_fs(i) {}
			virtual ~root_dir() = default;

			virtual void drop_weak_references() override;

			virtual meta_data metadata() override;
			virtual void metadata_change(const meta_data &info) override;
			virtual std::vector<dir_entry> contents() override;
			virtual file_t file_get(u64 key) override;
			virtual dir_t dir_get(u64 key) override;
			virtual file_t file_create(const meta_data &info) override;
			virtual void file_delete(u64 key) override;

			void update_file(u16 key, const u8 *entry);

		private:
			impl &m_fs;

			std::pair<fsblk_t::block_t, u32> get_dir_block(u64 key);
		};

		class file : public ifile_t {
		public:
			file(impl &fs, root_dir *dir, const u8 *entry, u16 key);
			virtual ~file() = default;

			virtual void drop_weak_references() override;

			virtual meta_data metadata() override;
			virtual void metadata_change(const meta_data &info) override;
			virtual std::vector<u8> read_all() override;
			virtual void replace(const std::vector<u8> &data) override;

		private:
			impl &m_fs;
			root_dir *m_dir;
			u16 m_key;
			u8 m_entry[18];
		};

		class system_file : public ifile_t {
		public:
			system_file(impl &fs, root_dir *dir, const u8 *entry, u16 key);
			virtual ~system_file() = default;

			virtual void drop_weak_references() override;

			virtual meta_data metadata() override;
			virtual void metadata_change(const meta_data &info) override;
			virtual std::vector<u8> read_all() override;
			virtual void replace(const std::vector<u8> &data) override;

		private:
			impl &m_fs;
			root_dir *m_dir;
			u16 m_key;
			u8 m_entry[18];
		};

		impl(fsblk_t &blockdev);
		virtual ~impl() = default;

		virtual void format(const meta_data &meta) override;
		virtual meta_data metadata() override;
		virtual void metadata_change(const meta_data &info) override;
		virtual dir_t root() override;

		static u32 cs_to_block(u16 ref);
		static u16 block_to_cs(u32 block);

		bool ref_valid(u16 ref);
		static std::string read_file_name(const u8 *p);
		void drop_root_ref();

		std::vector<u16> allocate_blocks(u32 count);
		void free_blocks(const std::vector<u16> &blocks);
		u32 free_block_count();

		static std::string file_name_prepare(std::string name);

	private:
		dir_t m_root;
	};

	oric_jasmin_image() : manager_t() {}

	virtual const char *name() const override;
	virtual const char *description() const override;

	virtual void enumerate_f(floppy_enumerator &fe, u32 form_factor, const std::vector<u32> &variants) const override;
	virtual std::unique_ptr<filesystem_t> mount(fsblk_t &blockdev) const override;

	virtual bool can_format() const override;
	virtual bool can_read() const override;
	virtual bool can_write() const override;
	virtual bool has_rsrc() const override;

	virtual std::vector<meta_description> volume_meta_description() const override;
	virtual std::vector<meta_description> file_meta_description() const override;

	static bool validate_filename(std::string name);
};

extern const oric_jasmin_image ORIC_JASMIN;

} // namespace fs

#endif
