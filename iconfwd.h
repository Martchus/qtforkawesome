#ifndef QT_FORK_AWESOME_ICONFWD
#define QT_FORK_AWESOME_ICONFWD

#include <QChar>

#include <type_traits>

namespace QtForkAwesome {

using IconBaseType = std::remove_reference_t<decltype(QChar().unicode())>;
enum class Icon : IconBaseType;

} // namespace QtForkAwesome

#endif // QT_FORK_AWESOME_ICONFWD
