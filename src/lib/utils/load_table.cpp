#include "load_table.hpp"

#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include "storage/table.hpp"

namespace opossum {

std::shared_ptr<Table> load_table(const std::string& file_name, size_t chunk_size) {
  std::ifstream infile(file_name);
  Assert(infile.is_open(), "load_table: Could not find file " + file_name);

  std::string line;
  std::getline(infile, line);
  std::vector<std::string> column_names = _split<std::string>(line, '|');
  std::getline(infile, line);
  std::vector<std::string> column_types = _split<std::string>(line, '|');

  std::shared_ptr<Table> test_table = std::make_shared<Table>(chunk_size);
  for (auto column_id = ColumnID{0}; column_id < column_names.size(); column_id++) {
    test_table->add_column(column_names[column_id], column_types[column_id]);
  }

  while (std::getline(infile, line)) {
    std::vector<AllTypeVariant> values = _split<AllTypeVariant>(line, '|');
    test_table->append(values);
  }
  return test_table;
}

}  // namespace opossum
