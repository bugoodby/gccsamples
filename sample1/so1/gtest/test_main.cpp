#include <gtest/gtest.h>
#include "test1.h"

namespace {

// �e�X�g�ΏۂƂȂ�N���X Foo �̂��߂̃t�B�N�X�`��
class FooTest : public ::testing::Test {
 protected:
  // �ȍ~�̊֐��Œ��g�̂Ȃ����͎̂��R�ɍ폜�ł��܂��D
  //

  FooTest() {
    // �e�X�g���Ɏ��s����� set-up �������ɏ����܂��D
  }

  virtual ~FooTest() {
    // �e�X�g���Ɏ��s�����C��O�𓊂��Ȃ� clean-up �������ɏ����܂��D
  }

  // �R���X�g���N�^�ƃf�X�g���N�^�ł͕s�\���ȏꍇ�D
  // �ȉ��̃��\�b�h���`���邱�Ƃ��ł��܂��F

virtual void SetUp() {
  // ���̃R�[�h�́C�R���X�g���N�^�̒���i�e�e�X�g�̒��O�j
  // �ɌĂяo����܂��D
}

virtual void TearDown() {
  // ���̃R�[�h�́C�e�e�X�g�̒���i�f�X�g���N�^�̒��O�j
  // �ɌĂяo����܂��D
}

// �����Ő錾�����I�u�W�F�N�g�́C�e�X�g�P�[�X���̑S�Ẵe�X�g�ŗ��p�ł��܂��D
};


TEST(FooTest, say_hello) {
    EXPECT_EQ( say_hello(), 0 );
}

}
