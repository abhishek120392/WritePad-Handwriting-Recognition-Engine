/***************************************************************************************
 *
 *  WRITEPAD(r): Handwriting Recognition Engine (HWRE) and components.
 *  Copyright (c) 2001-2016 PhatWare (r) Corp. All rights reserved.
 *
 *  Licensing and other inquires: <developer@phatware.com>
 *  Developer: Stan Miasnikov, et al. (c) PhatWare Corp. <http://www.phatware.com>
 *
 *  WRITEPAD HWRE is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU "AS-IS"
 *  AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR OTHERWISE,
 *  INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY OR
 *  FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL PHATWARE CORP.
 *  BE LIABLE TO YOU OR ANYONE ELSE FOR ANY DIRECT, SPECIAL, INCIDENTAL,
 *  INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER,
 *  INCLUDING WITHOUT LIMITATION, LOSS OF PROFIT, LOSS OF USE, SAVINGS
 *  OR REVENUE, OR THE CLAIMS OF THIRD PARTIES, WHETHER OR NOT PHATWARE CORP.
 *  HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH LOSS, HOWEVER CAUSED AND ON
 *  ANY THEORY OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE
 *  POSSESSION, USE OR PERFORMANCE OF THIS SOFTWARE.
 *  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with WritePad.  If not, see <http://www.gnu.org/licenses/>.
 *
 **************************************************************************************/

#ifndef MAIN_HEADER_INCLUDED
#define MAIN_HEADER_INCLUDED

// -------------------- Defines ------------------------------------------------

#define ver_h "MLP analysis lab v 1.03"
#define SNN_DATA_VER "SNN data file Ver. 1.7."

#define MAIN_FILENAME_LEN 128

#define MAIN_SIZE_SAMPLE   16
#define MAIN_NUMSYMINSAMP   8
#define MAIN_MAX_STEP      0.2
#define MAIN_SIZE_BLOCK (64*1024)
#define MAIN_MAX_BLOCKS   1000

#define MAIN_MLP_INI_NAME  "mlp.ini"
#define MAIN_DMP_NET_NAME  "net.dmp"

#define DEF_PARAM_NAMES    {                               \
                           "Step_control_mode    ",        \
                           "Step_coeff           ",        \
                           "Step_decay_coeff     ",        \
                           "Step_min             ",        \
                           "Step_layer_coeff_1   ",        \
                           "Step_layer_coeff_2   ",        \
                           "Step_layer_coeff_3   ",        \
                           "Step_layer_coeff_4   ",        \
                           "Step_coeff_1         ",        \
                           "Step_coeff_2         ",        \
                           "Step_coeff_3         ",        \
                           "Step_coeff_4         ",        \
                           "Logistic_coeff       ",        \
                           "Logistic_decay_coeff ",        \
                           "Logistic_max         ",        \
                           "Weigth_init_coeff    ",        \
                           "Inert_coeff          ",        \
                           "Zero_ballance_coeff  ",        \
                           "Epoch_size           ",        \
                           "Epoch_size_decay     ",        \
                           "Epoch_size_max       ",        \
                           "Testing_rate         ",        \
                           "Learn_data_file      ",        \
                           "Test_data_file       ",        \
                           "OSym_bush_size ",              \
                           ""                              \
                           }

// -------------------- Structures and classes ---------------------------------

typedef unsigned char uchar;

typedef struct
{
	uchar  flags;
	uchar  nsym;
	uchar  nvs[MAIN_NUMSYMINSAMP/2];
	uchar  syms[MAIN_NUMSYMINSAMP];
	uchar  data[MAIN_SIZE_SAMPLE];
} sample_type, * p_sample_type;

typedef struct
{
	int   step_mode;
	float step_size;
	float step_decay_coeff;
	float step_min;
	float step_layer_coeffs[MLP_NET_NUMLAYERS];
	float step_coeffs[MLP_NET_NUMLAYERS];

	float logistic_coeff;
	float logistic_decay_coeff;
	float logistic_max;

	float weigth_init_coeff;

	float inert_coeff;

	float zero_ballance_coeff;

	float epoch_size;
	float epoch_size_decay;
	float epoch_size_max;

	int   testing_rate;

	char  learn_data_file[128];
	char  test_data_file[128];

	uchar symco[MLP_NET_NUMOUTPUTS];
	uchar bushes[MLP_NET_NUMOUTPUTS];

} params_type, * p_params_type;

class samples_storage_class
{
	int num_blocks;
	int num_in_block;
	int max_in_block;
	p_sample_type blocks[MAIN_MAX_BLOCKS];

public:

	int num_samples;
	int num_antisamp;
	int counter;

	samples_storage_class();
	~samples_storage_class();
	AddSample(p_sample_type sample);
	GetSample(int num, p_sample_type * pp_sample);
	ResetSampleFlags(uchar flags);
	SetSampleFlags(int num, uchar flags);
	GetSampleFlags(int num);

	friend ostream& operator << (ostream& os, samples_storage_class *storage);
};

typedef struct
{
	uchar rsym;
	float w;
	int   ns;
	int   nv;
	int   points[2][32][2];
	uchar syms[MAIN_NUMSYMINSAMP];
} view_dialog_type, * p_view_dialog_type;

// -------------------- Function definitions -----------------------------------

int   MainLearnNet(int mode);
int   MainTestNet(int mode);
int   MainAnalyzeData(int mode);
int   MainDumpNetImage(int mode);

int   AskForParams(p_params_type pm, float * shake);
int   InitParams(int mode, char * fname, p_params_type pm);

int   PS_Yield(void);

int   LoadSamples(char * data_name, samples_storage_class * samples);
float LearnNet(mlp_net_class * net, samples_storage_class * samples, p_params_type mp);
int   CreateDesiredOutputs(uchar corr_sym, uchar * syms, uchar * symco, float * desired_outputs);
float TestNet(mlp_net_class * net, samples_storage_class * samples, char * log_name, int mode, float lfc);
int   AdjustStepCoeffs(mlp_net_class * net, p_params_type pm);
int   LogSamplesStat(samples_storage_class * samples, char * log_name);
int   WriteBackSomeSamples(samples_storage_class * samples, char * log_name);
int   DumpNetImage(char * dat_name, mlp_net_class * net);

int   GetInputs(int num, uchar *syms, uchar * inputs, samples_storage_class * samples);
int   GetInputsAtRandom(int flags, uchar *what, uchar * inps, uchar * syms, int * ns, samples_storage_class * samples);
int   GetInputsSequencial(int flags, uchar *what, uchar * inps, uchar * syms, int * ns, samples_storage_class * samples);
int   RockCoeffs(int mode, uchar * inps);

int   DrawCoeff(uchar rsym, float w, int num, samples_storage_class * samples, uchar * coeff);


#endif // MAIN_HEADER_INCLUDED
/* **************************************************************************** */
/* *        End OF all                                                        * */
/* **************************************************************************** */
//
