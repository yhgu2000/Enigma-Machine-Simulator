#pragma once
#include <QString>
#include <array>

/** 【设计说明】
 * 这个地方把 26 拎出来写成一个常量，并不是为了方便以后的扩展。
 * 而是为了防止在编写代码时输错，确保代码各处的值一致。同时也修改了
 * 数值的类型，利用了 C++ 的类型系统。
 *
 * 综合来看，好处多多，应该多用！
 * 经验：“使用不只一次的字面值应该定义为常量”
 */
static constexpr uint8_t kRotorMod = 26;

using rotor_t = std::array<uint8_t, kRotorMod>;

class Rotor : public rotor_t
{
  static const rotor_t kDefault_t;

public:
  static const Rotor& kDefault;

public:
  Rotor()
    : rotor_t(kDefault)
  {}
  Rotor(const Rotor&) = default;

public:
  QString hash();

  /**
   * @brief reverse_to
   * @param r
   * @abstract 构造 r 为与自己对称的转子
   */
  void reverse_to(Rotor& r);

  /**
   * @brief encode
   * @param s 输出字符串
   * @return 成功返回 true 失败返回 false
   */
  bool encode(QString& s) const;

  /**
   * @brief decode
   * @param s
   * @return 成功返回 true，失败返回 false
   * @abstract 如果失败，原 Rotor 对象不会被修改（安全代码设计）
   */
  bool decode(const QString& s);
};
