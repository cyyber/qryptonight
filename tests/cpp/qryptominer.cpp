/*
  * This program is free software: you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation, either version 3 of the License, or
  * any later version.
  *
  * This program is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with this program.  If not, see <http://www.gnu.org/licenses/>.
  *
  * Additional permission under GNU GPL version 3 section 7
  *
  * If you modify this Program, or any covered work, by linking or combining
  * it with OpenSSL (or a modified version of that library), containing parts
  * covered by the terms of OpenSSL License and SSLeay License, the licensors
  * of this Program grant you additional permission to convey the resulting work.
  *
  */
#include <iostream>
#include <qryptonight/qryptominer.h>
#include <misc/bignum.h>
#include <pow/powhelper.h>
#include <qryptonight/qryptonight.h>
#include "gtest/gtest.h"

namespace {
TEST(Qryptominer, PassesTarget)
{
    Qryptominer qm;

    std::vector<uint8_t> target{
            0x3E, 0xE5, 0x3F, 0xE1, 0xAC, 0xF3, 0x55, 0x92,
            0x66, 0xD8, 0x43, 0x89, 0xCE, 0xDE, 0x99, 0x33,
            0xC6, 0x8F, 0xC5, 0x1E, 0xD0, 0xA6, 0xC7, 0x91,
            0xF8, 0xF9, 0xE8, 0x9D, 0xB6, 0x23, 0xF0, 0xF6
    };

    ASSERT_TRUE(PoWHelper::passesTarget(target, target));

    std::cout << std::endl;
    std::cout << printByteVector2(target) << std::endl;

    // Iterate changing a single byte
    for (int i = 0; i<32; i++) {
        std::vector<uint8_t> below_1 = target;
        below_1[31-i]--;

        std::cout << printByteVector2(below_1) << std::endl;
        ASSERT_TRUE(PoWHelper::passesTarget(below_1, target));

        std::vector<uint8_t> over_1 = target;
        over_1[31-i]++;
        std::cout << printByteVector2(over_1) << std::endl;
        ASSERT_FALSE(PoWHelper::passesTarget(over_1, target));
    }
}

TEST(Qryptominer, Run1Thread)
{
    Qryptominer qm;

    std::vector<uint8_t> input{
            0x03, 0x05, 0x07, 0x09, 0x03, 0x05, 0x07, 0x09,
            0x03, 0x05, 0x07, 0x09, 0x03, 0x05, 0x07, 0x09,
            0x03, 0x05, 0x07, 0x09, 0x03, 0x05, 0x07, 0x09,
            0x03, 0x05, 0x07, 0x09, 0x03, 0x05, 0x07, 0x09,
            0x03, 0x05, 0x07, 0x09, 0x03, 0x05, 0x07, 0x09,
            0x03, 0x05, 0x07, 0x09, 0x03, 0x05, 0x07, 0x09,
            0x03, 0x05, 0x07, 0x09, 0x03, 0x05, 0x07, 0x09,
            0x03, 0x05, 0x07, 0x09, 0x03, 0x05, 0x07, 0x09
    };

    std::vector<uint8_t> target = {
            0x0F, 0xFF, 0xFF, 0xE1, 0xAC, 0xF3, 0x55, 0x92,
            0x66, 0xD8, 0x43, 0x89, 0xCE, 0xDE, 0x99, 0x33,
            0xC6, 0x8F, 0xC5, 0x1E, 0xD0, 0xA6, 0xC7, 0x91,
            0xF8, 0xF9, 0xE8, 0x9D, 0xB6, 0x23, 0xF0, 0x0F
    };

    qm.start(input, 0, target);

    qm.waitForAnswer(60);

    ASSERT_TRUE(qm.solutionAvailable());

    EXPECT_EQ(37, qm.solutionNonce());
    std::cout << printByteVector(qm.solutionHash()) << std::endl;

    std::vector<uint8_t> expected_winner{
            0x00, 0x00, 0x00, 0x25, 0x03, 0x05, 0x07, 0x09,
            0x03, 0x05, 0x07, 0x09, 0x03, 0x05, 0x07, 0x09,
            0x03, 0x05, 0x07, 0x09, 0x03, 0x05, 0x07, 0x09,
            0x03, 0x05, 0x07, 0x09, 0x03, 0x05, 0x07, 0x09,
            0x03, 0x05, 0x07, 0x09, 0x03, 0x05, 0x07, 0x09,
            0x03, 0x05, 0x07, 0x09, 0x03, 0x05, 0x07, 0x09,
            0x03, 0x05, 0x07, 0x09, 0x03, 0x05, 0x07, 0x09,
            0x03, 0x05, 0x07, 0x09, 0x03, 0x05, 0x07, 0x09
    };

    std::vector<uint8_t> expected_hash{
            0x59, 0x57, 0x8b, 0x3d, 0x5d, 0x91, 0x86, 0x12,
            0xa4, 0x74, 0x67, 0xeb, 0xe4, 0xbd, 0x84, 0xa2,
            0x89, 0x98, 0x4d, 0x1b, 0x5b, 0xa5, 0x3e, 0x30,
            0x84, 0x05, 0xc9, 0xcd, 0x7e, 0xab, 0x44, 0x0a
    };

    EXPECT_EQ(expected_winner, qm.solutionInput());

    std::cout << printByteVector2(qm.solutionHash()) << std::endl;

    EXPECT_EQ(expected_hash, qm.solutionHash());
}

TEST(Qryptominer, RunThreads_KeepHashing)
{
    Qryptominer qm;

    std::vector<uint8_t> input(80);
    std::vector<uint8_t> boundary = {
            0x00, 0x06, 0xFF, 0xE1, 0xAC, 0xF3, 0x55, 0x92,
            0x66, 0xD8, 0x43, 0x89, 0xCE, 0xDE, 0x99, 0x33,
            0xC6, 0x8F, 0xC5, 0x1E, 0xD0, 0xA6, 0xC7, 0x91,
            0xF8, 0xF9, 0xE8, 0x9D, 0xB6, 0x23, 0x06, 0x00
    };

    qm.start(input, 0, boundary, 10);

    int hash_count = 0;
    while (!qm.solutionAvailable()) {
        Qryptonight qn;
        EXPECT_TRUE(qn.isValid());

        std::vector<uint8_t> internal_input(10000);

        std::vector<uint8_t> output_expected{
                0xbf, 0x2f, 0xa0, 0x5d, 0x59, 0x67, 0x60, 0xa8,
                0x43, 0x19, 0xd9, 0xe8, 0x97, 0x5e, 0x80, 0xcc,
                0xa6, 0x70, 0xdd, 0x9e, 0x19, 0xc8, 0x37, 0x3d,
                0xc9, 0x05, 0x49, 0xf2, 0xd3, 0x09, 0x5e, 0x0c
        };

        auto output = qn.hash(internal_input);
        EXPECT_EQ(output_expected, output);
        hash_count++;
    }

    std::cout << hash_count << std::endl;

    ASSERT_TRUE(qm.solutionAvailable());
    EXPECT_EQ(1346, qm.solutionNonce());
}

TEST(Qryptominer, Run1Thread_bigblob)
{
    Qryptominer qm;
    std::vector<uint8_t> input(1000);

    std::vector<uint8_t> boundary = {
            0x0F, 0xFF, 0xFF, 0xE1, 0xAC, 0xF3, 0x55, 0x92,
            0x66, 0xD8, 0x43, 0x89, 0xCE, 0xDE, 0x99, 0x33,
            0xC6, 0x8F, 0xC5, 0x1E, 0xD0, 0xA6, 0xC7, 0x91,
            0xF8, 0xF9, 0xE8, 0x9D, 0xB6, 0x23, 0xF0, 0x0F
    };

    qm.start(input, 0, boundary);

    std::cout << "miner started" << std::endl;
    qm.waitForAnswer(60);

    ASSERT_TRUE(qm.solutionAvailable());
    EXPECT_EQ(50, qm.solutionNonce());
}

TEST(Qryptominer, RunAndRestart)
{
    Qryptominer qm;

    std::vector<uint8_t> input{
            0x03, 0x05, 0x07, 0x09, 0x03, 0x05, 0x07, 0x09,
            0x03, 0x05, 0x07, 0x09, 0x03, 0x05, 0x07, 0x09,
            0x03, 0x05, 0x07, 0x09, 0x03, 0x05, 0x07, 0x09,
            0x03, 0x05, 0x07, 0x09, 0x03, 0x05, 0x07, 0x09,
            0x03, 0x05, 0x07, 0x09, 0x03, 0x05, 0x07, 0x09,
            0x03, 0x05, 0x07, 0x09, 0x03, 0x05, 0x07, 0x09,
            0x03, 0x05, 0x07, 0x09, 0x03, 0x05, 0x07, 0x09,
            0x03, 0x05, 0x07, 0x09, 0x03, 0x05, 0x07, 0x09
    };

    std::vector<uint8_t> boundary = {
            0x0F, 0xFF, 0xFF, 0xE1, 0xAC, 0xF3, 0x55, 0x92,
            0x66, 0xD8, 0x43, 0x89, 0xCE, 0xDE, 0x99, 0x33,
            0xC6, 0x8F, 0xC5, 0x1E, 0xD0, 0xA6, 0xC7, 0x91,
            0xF8, 0xF9, 0xE8, 0x9D, 0xB6, 0x23, 0xF0, 0x0F
    };

    qm.start(input, 0, boundary);
    qm.waitForAnswer(60);

    ASSERT_TRUE(qm.solutionAvailable());
    EXPECT_EQ(37, qm.solutionNonce());

    std::vector<uint8_t> expected_winner{
            0x00, 0x00, 0x00, 0x25, 0x03, 0x05, 0x07, 0x09,
            0x03, 0x05, 0x07, 0x09, 0x03, 0x05, 0x07, 0x09,
            0x03, 0x05, 0x07, 0x09, 0x03, 0x05, 0x07, 0x09,
            0x03, 0x05, 0x07, 0x09, 0x03, 0x05, 0x07, 0x09,
            0x03, 0x05, 0x07, 0x09, 0x03, 0x05, 0x07, 0x09,
            0x03, 0x05, 0x07, 0x09, 0x03, 0x05, 0x07, 0x09,
            0x03, 0x05, 0x07, 0x09, 0x03, 0x05, 0x07, 0x09,
            0x03, 0x05, 0x07, 0x09, 0x03, 0x05, 0x07, 0x09
    };

    EXPECT_EQ(expected_winner, qm.solutionInput());
}

TEST(Qryptominer, MeasureHashRate)
{
    Qryptominer qm;

    std::vector<uint8_t> input{
            0x03, 0x05, 0x07, 0x09, 0x03, 0x05, 0x07, 0x09,
            0x03, 0x05, 0x07, 0x09, 0x03, 0x05, 0x07, 0x09,
            0x03, 0x05, 0x07, 0x09, 0x03, 0x05, 0x07, 0x09,
            0x03, 0x05, 0x07, 0x09, 0x03, 0x05, 0x07, 0x09,
            0x03, 0x05, 0x07, 0x09, 0x03, 0x05, 0x07, 0x09,
            0x03, 0x05, 0x07, 0x09, 0x03, 0x05, 0x07, 0x09,
            0x03, 0x05, 0x07, 0x09, 0x03, 0x05, 0x07, 0x09,
            0x03, 0x05, 0x07, 0x09, 0x03, 0x05, 0x07, 0x09
    };

    std::vector<uint8_t> target{
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x66, 0xD8, 0x43, 0x89, 0xCE, 0xDE, 0x99, 0x33,
            0xC6, 0x8F, 0xC5, 0x1E, 0xD0, 0xA6, 0xC7, 0x91,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    };

    qm.start(input, 0, target, 4);
    qm.waitForAnswer(3);
    std::cout << std::endl << "hashes/sec: " << qm.hashRate() << std::endl;

    EXPECT_FALSE(qm.solutionAvailable());
    qm.cancel();
    ASSERT_FALSE(qm.isRunning());
}

TEST(Qryptominer, RunAndCancel)
{
    Qryptominer qm;

    std::vector<uint8_t> input{
            0x03, 0x05, 0x07, 0x09, 0x03, 0x05, 0x07, 0x09,
            0x03, 0x05, 0x07, 0x09, 0x03, 0x05, 0x07, 0x09,
            0x03, 0x05, 0x07, 0x09, 0x03, 0x05, 0x07, 0x09,
            0x03, 0x05, 0x07, 0x09, 0x03, 0x05, 0x07, 0x09,
            0x03, 0x05, 0x07, 0x09, 0x03, 0x05, 0x07, 0x09,
            0x03, 0x05, 0x07, 0x09, 0x03, 0x05, 0x07, 0x09,
            0x03, 0x05, 0x07, 0x09, 0x03, 0x05, 0x07, 0x09,
            0x03, 0x05, 0x07, 0x09, 0x03, 0x05, 0x07, 0x09
    };

    std::vector<uint8_t> boundary = {
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x66, 0xD8, 0x43, 0x89, 0xCE, 0xDE, 0x99, 0x33,
            0xC6, 0x8F, 0xC5, 0x1E, 0xD0, 0xA6, 0xC7, 0x91,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    };

    qm.start(input, 0, boundary);
    qm.waitForAnswer(1);
    qm.cancel();
    ASSERT_FALSE(qm.isRunning());

    ASSERT_FALSE(qm.solutionAvailable());
}

TEST(Qryptominer, RunCancelSafety)
{
    Qryptominer qm;

    std::vector<uint8_t> input{
            0x03, 0x05, 0x07, 0x09, 0x03, 0x05, 0x07, 0x09,
            0x03, 0x05, 0x07, 0x09, 0x03, 0x05, 0x07, 0x09,
            0x03, 0x05, 0x07, 0x09, 0x03, 0x05, 0x07, 0x09,
            0x03, 0x05, 0x07, 0x09, 0x03, 0x05, 0x07, 0x09,
            0x03, 0x05, 0x07, 0x09, 0x03, 0x05, 0x07, 0x09,
            0x03, 0x05, 0x07, 0x09, 0x03, 0x05, 0x07, 0x09,
            0x03, 0x05, 0x07, 0x09, 0x03, 0x05, 0x07, 0x09,
            0x03, 0x05, 0x07, 0x09, 0x03, 0x05, 0x07, 0x09
    };

    std::vector<uint8_t> boundary = {
            0x9F, 0xFF, 0xFF, 0xE1, 0xAC, 0xF3, 0x55, 0x92,
            0x66, 0xD8, 0x43, 0x89, 0xCE, 0xDE, 0x99, 0x33,
            0xC6, 0x8F, 0xC5, 0x1E, 0xD0, 0xA6, 0xC7, 0x91,
            0xF8, 0xF9, 0xE8, 0x9D, 0xB6, 0x23, 0xF0, 0xFF
    };

    for (int i = 0; i<20; i++) {
        using namespace std::chrono_literals;
        boundary[0] -= 10;
        qm.start(input, 0, boundary);
        std::this_thread::sleep_for(500ms);
        qm.cancel();
    }

    ASSERT_FALSE(qm.isRunning());
}

}
