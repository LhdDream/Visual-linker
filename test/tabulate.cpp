#include "tabulate/markdown_exporter.hpp"  // 注意这个头文件
#include "tabulate/table.hpp"
using namespace tabulate;
using namespace std;
int main() {
  Table hellogithub;
  hellogithub.add_row({"HelloGitHub"});
  hellogithub[0][0].format().font_style({FontStyle::bold}); // 加粗样式，在 Markdown 中可以表现出来
  hellogithub.add_row({"GitHub repo: github.com/521xueweihan/HelloGitHub"});
  hellogithub.add_row({"Website: hellogithub.com"});

  // 导出为 Markdown
  MarkdownExporter exporter;
  auto markdown = exporter.dump(hellogithub);

  cout << hellogithub << endl << endl;
  cout << "Markdown Source:\n\n" << markdown << endl;
  return 0;
}