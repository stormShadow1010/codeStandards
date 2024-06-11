using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;

namespace LaserWrit
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void goButton_Click(object sender, EventArgs e)
        {
            // TODO Create the file based on the input text box.
            writeFile(inputTextBox.Text.ToUpper());
        }
        private void writeFile(string inText)
        {

            // Pre processing on text string
            // all lines must be 10 characters long
            while (inText.Length < 10)
            {
                inText += " ";
            }

            //Convert to binary
            Queue<byte> byteQueue = textToBin(inText);
            //Write binary to a file
            writeBinFile(byteQueue);
            //MessageBox.Show(inText);
        }

        // This function takes a text string and converts it to the propper bytes
        // They bytes are stored into a queue for easy processing
        private Queue<byte> textToBin(string text){

            // declarations to be filled later
            Queue<byte> ret = new Queue<byte>();
            Queue<byte> temp;
            letterLib lib = new letterLib();
            List<point> btemp;
            List<point> bnotsotemp;
            int col = 0; // keeps track of what column the letter will be in
            int writTimes = 3; // This is how many times the letter will be written before moving on

            //Read in the text and get place it into the queue
            foreach (char let in text)
            {
                int rewrite = 0;
                // get the point list for the character
                btemp = lib.diction[let];



                while (rewrite < writTimes)
                {

                    int addToCol = lib.xMaxPerLet * col;

                    foreach (point tempChar in btemp)
                    {
                        point tempPoint = new point(tempChar.x, tempChar.y);

                        // alter the character for position
                        tempPoint.x += lib.xMaxPerLet * col;

                        // change the altered point to bytes
                        temp = lib.genBytes(tempPoint);

                        // stick the bytes into the queue.
                        
                        
                        foreach (byte by in temp)
                        {
                            ret.Enqueue(by);
                        } 
                        


                    }
 
                    rewrite++;
                }
                // add a deliminator for the end of each letter
                // del is 4 bytes only the first has to be 0xF
                ret.Enqueue((byte)255);
                ret.Enqueue((byte)255);
                ret.Enqueue((byte)255);
                ret.Enqueue((byte)255);

                // update the col position
                col++;

            }

            //  return the queue
            return ret;

        }

        // This function takes a binary queue file and writes it to a .dat
        private void writeBinFile(Queue<byte> byteQueue){
            FileStream writeStream ;
            try
            {
                MessageBox.Show("The data file is located at:\n"+ Environment.GetFolderPath(Environment.SpecialFolder.Desktop));
                writeStream = new FileStream(Environment.GetFolderPath(Environment.SpecialFolder.Desktop)+"\\laser.dat", FileMode.Create);
                BinaryWriter writeBinay = new BinaryWriter(writeStream);
                while(byteQueue.Count > 0){
                    writeBinay.Write(byteQueue.Dequeue());
                }
                writeBinay.Close();
            }
            catch (Exception ex)
            {
                MessageBox.Show (ex.ToString());
            }
        }


        
            
    }
   
}
