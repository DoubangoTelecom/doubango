namespace test
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.buttonRegister = new System.Windows.Forms.Button();
            this.buttonUnRegister = new System.Windows.Forms.Button();
            this.textBoxDebug = new System.Windows.Forms.TextBox();
            this.buttonPublish = new System.Windows.Forms.Button();
            this.buttonUnPublish = new System.Windows.Forms.Button();
            this.buttonUnSub = new System.Windows.Forms.Button();
            this.buttonSub = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // buttonRegister
            // 
            this.buttonRegister.Location = new System.Drawing.Point(12, 263);
            this.buttonRegister.Name = "buttonRegister";
            this.buttonRegister.Size = new System.Drawing.Size(75, 23);
            this.buttonRegister.TabIndex = 0;
            this.buttonRegister.Text = "Register";
            this.buttonRegister.UseVisualStyleBackColor = true;
            this.buttonRegister.Click += new System.EventHandler(this.buttonRegister_Click);
            // 
            // buttonUnRegister
            // 
            this.buttonUnRegister.Location = new System.Drawing.Point(199, 263);
            this.buttonUnRegister.Name = "buttonUnRegister";
            this.buttonUnRegister.Size = new System.Drawing.Size(75, 23);
            this.buttonUnRegister.TabIndex = 1;
            this.buttonUnRegister.Text = "UnRegister";
            this.buttonUnRegister.UseVisualStyleBackColor = true;
            this.buttonUnRegister.Click += new System.EventHandler(this.buttonUnRegister_Click);
            // 
            // textBoxDebug
            // 
            this.textBoxDebug.Location = new System.Drawing.Point(12, 292);
            this.textBoxDebug.Multiline = true;
            this.textBoxDebug.Name = "textBoxDebug";
            this.textBoxDebug.Size = new System.Drawing.Size(262, 70);
            this.textBoxDebug.TabIndex = 2;
            // 
            // buttonPublish
            // 
            this.buttonPublish.Location = new System.Drawing.Point(12, 234);
            this.buttonPublish.Name = "buttonPublish";
            this.buttonPublish.Size = new System.Drawing.Size(75, 23);
            this.buttonPublish.TabIndex = 3;
            this.buttonPublish.Text = "Publish";
            this.buttonPublish.UseVisualStyleBackColor = true;
            this.buttonPublish.Click += new System.EventHandler(this.buttonPublish_Click);
            // 
            // buttonUnPublish
            // 
            this.buttonUnPublish.Location = new System.Drawing.Point(199, 235);
            this.buttonUnPublish.Name = "buttonUnPublish";
            this.buttonUnPublish.Size = new System.Drawing.Size(75, 23);
            this.buttonUnPublish.TabIndex = 4;
            this.buttonUnPublish.Text = "UnPublish";
            this.buttonUnPublish.UseVisualStyleBackColor = true;
            this.buttonUnPublish.Click += new System.EventHandler(this.buttonUnPublish_Click);
            // 
            // buttonUnSub
            // 
            this.buttonUnSub.Location = new System.Drawing.Point(199, 206);
            this.buttonUnSub.Name = "buttonUnSub";
            this.buttonUnSub.Size = new System.Drawing.Size(75, 23);
            this.buttonUnSub.TabIndex = 6;
            this.buttonUnSub.Text = "UnSubscribe";
            this.buttonUnSub.UseVisualStyleBackColor = true;
            // 
            // buttonSub
            // 
            this.buttonSub.Location = new System.Drawing.Point(12, 205);
            this.buttonSub.Name = "buttonSub";
            this.buttonSub.Size = new System.Drawing.Size(75, 23);
            this.buttonSub.TabIndex = 5;
            this.buttonSub.Text = "Subscribe";
            this.buttonSub.UseVisualStyleBackColor = true;
            this.buttonSub.Click += new System.EventHandler(this.buttonSub_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(286, 374);
            this.Controls.Add(this.buttonUnSub);
            this.Controls.Add(this.buttonSub);
            this.Controls.Add(this.buttonUnPublish);
            this.Controls.Add(this.buttonPublish);
            this.Controls.Add(this.textBoxDebug);
            this.Controls.Add(this.buttonUnRegister);
            this.Controls.Add(this.buttonRegister);
            this.Name = "Form1";
            this.Text = "Form1";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button buttonRegister;
        private System.Windows.Forms.Button buttonUnRegister;
        private System.Windows.Forms.TextBox textBoxDebug;
        private System.Windows.Forms.Button buttonPublish;
        private System.Windows.Forms.Button buttonUnPublish;
        private System.Windows.Forms.Button buttonUnSub;
        private System.Windows.Forms.Button buttonSub;
    }
}

